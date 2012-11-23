//------------------------------------------------------------------------------
//  DAE2Nebula3Writer_Animation.cpp
//  (C) 2010 Dengwenyi
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "DAE2Nebula3Writer.h"

#include "COLLADASaxFWLLoader.h"
#include "COLLADAFWRoot.h"
#include "COLLADAFWGeometry.h"
#include "COLLADAFWNode.h"
#include "COLLADAFWVisualScene.h"
#include "COLLADAFWInstanceGeometry.h"
#include "COLLADAFWMesh.h"
#include "COLLADAFWRotate.h"
#include "COLLADAFWScale.h"
#include "COLLADAFWTranslate.h"
#include "COLLADAFWMatrix.h"
#include "COLLADAFWSkinControllerData.h"
#include "Math/COLLADABUMathMatrix4.h"

#include "io/ioserver.h"
#include "io/stream.h"
#include "modelnodewriter.h"

namespace Models
{
	using namespace COLLADAFW;
	using namespace IO;
	using namespace ToolkitUtil;
	using namespace Characters;

	//--------------------------------------------------------------------
	void Nebula3Writer::importAnimationCurve ( COLLADAFW::AnimationCurve* animationCurve ,AnimationData& values)
	{
		// The input is always the time 
		const COLLADAFW::FloatOrDoubleArray& inputValuesArray = animationCurve->getInputValues ();
		const COLLADAFW::ArrayPrimitiveType<float>* inValues = inputValuesArray.getFloatValues ();
		size_t numInputValues = inputValuesArray.getValuesCount ();

		// The output can have different dimensions.
		const COLLADAFW::FloatOrDoubleArray& outputValuesArray = animationCurve->getOutputValues ();
		const COLLADAFW::ArrayPrimitiveType<float>* outValues = outputValuesArray.getFloatValues ();
		size_t outDimension = animationCurve->getOutDimension ();
		size_t numOutputValues = outputValuesArray.getValuesCount () / outDimension;

		size_t keyCount = animationCurve->getKeyCount ();
		if ( numInputValues != numOutputValues || numInputValues != keyCount )
		{
			n_error("Nebula3Writer::importAnimationCurve(): Invalid animation!");
			return;
		}

		values.outDimension = outDimension;
		for ( size_t inputIndex=0; inputIndex<keyCount; ++inputIndex )
		{
			float inputValue = (*inValues)[inputIndex];			
			values.times.Append(inputValue);

			for( size_t j = 0; j< outDimension; j++)
			{
				float outValue = (*outValues)[inputIndex * outDimension + j];
				values.values.Append(outValue);
			}			
		}///end for
	}

	//--------------------------------------------------------------------
	void Nebula3Writer::importAnimationList( const COLLADAFW::AnimationList* animationList )
	{
		const COLLADAFW::UniqueId& animationListId = animationList->getUniqueId ();
		//n_printf("importAnimationList [%s]......\n",animationListId.toAscii().c_str());

		if( this->mAnimationListDatas.Contains(animationListId)) return;		
		Util::Array<Math::matrix44> animationMatrixs;

		/// 合并计算
		const AnimationList::AnimationBindings& animationBindings = animationList->getAnimationBindings();		
		for(unsigned int i =0; i< animationBindings.getCount();i++)
		{
			const COLLADAFW::UniqueId& animationId = animationBindings.getData()[i].animation;
			if( !this->mAnimationDatas.Contains(animationId) ) continue;
			AnimationData& animData = mAnimationDatas[animationId];
			SizeT size = animData.values.Size();

			switch(animationBindings.getData()[i].animationClass)
			{
			case AnimationList::MATRIX4X4:
				{
					size_t outDimension = animData.outDimension;
					for (unsigned int j =0; j < size/outDimension ;j++ )
					{
						COLLADABU::Math::Matrix4 temp;
						for (unsigned int k=0;k< outDimension;k++)
						{
							temp.setElement(k,animData.values[j*outDimension + k]);
						}///end for
						animationMatrixs.Append(this->toMatrix44(temp));
					}
				}
				break;
			default:
				n_error("Nebula3Writer::writeAnimationList [animationClass type not support!]\n");
				break;
			}///end switch
		}///end for

		if(!animationMatrixs.IsEmpty())
			this->mAnimationListDatas.Add(animationListId,animationMatrixs);
	}

	//--------------------------------------------------------------------
	Util::String Nebula3Writer::writeAnimationData( const CharacterData& characterData ,const JointMaps& jointMaps)
	{
		/// 合并关键帧
		Util::String animFilename = ANIM_PRE_FIXED;
		animFilename.Append(characterData.characterName);
		animFilename.Append("_skinned.nax3");
		AnimBuilder animBuilder;
		AnimBuilderClip clip;

		/// 找到keyCount			
		Util::Array<COLLADAFW::UniqueId> keys = this->mAnimationListDatas.KeysAsArray();
		n_assert(!keys.IsEmpty());
		SizeT keyCount = 0;
		for ( int i=0;i<keys.Size();i++ )
		{
			Util::Array<Math::matrix44>& animationKeys = this->mAnimationListDatas[keys[i]];
			if( keyCount < animationKeys.Size()) keyCount = animationKeys.Size();
		}///end for

		const SkinData& skinData = this->mSkinDatas[characterData.skinUniqueId];
		const Util::Array<COLLADAFW::UniqueId>& jointUniqueIds = jointMaps.KeysAsArray();
		for( int i =0; i< jointUniqueIds.Size(); i++ )
		{
			const COLLADAFW::UniqueId& jointId = jointUniqueIds[i];
			AnimBuilderCurve curveTranslation;
			curveTranslation.SetActive(true);
			curveTranslation.ResizeKeyArray(keyCount);
			curveTranslation.SetCurveType(CoreAnimation::CurveType::Translation);			
			curveTranslation.SetFirstKeyIndex( i * 3 );

			AnimBuilderCurve curveRotation;
			curveRotation.SetActive(true);
			curveRotation.ResizeKeyArray(keyCount);
			curveRotation.SetCurveType(CoreAnimation::CurveType::Rotation);
			curveRotation.SetFirstKeyIndex( i * 3 + 1 );

			AnimBuilderCurve curveScale;
			curveScale.SetActive(true);
			curveScale.ResizeKeyArray(keyCount);
			curveScale.SetCurveType(CoreAnimation::CurveType::Scale);
			curveScale.SetFirstKeyIndex( i * 3 + 2 );	

			NodeData& nodeData = this->mNodeMaps[jointId];
			COLLADAFW::UniqueId animationListId = COLLADAFW::UniqueId::INVALID;
			if( !nodeData.animationListUniqueIds.IsEmpty())
				animationListId = nodeData.animationListUniqueIds[0];
			Math::matrix44 matJoint = characterData.jointMatrix[jointId];

			for (int keyIndex=0;keyIndex< keyCount;keyIndex++)
			{						
				Math::matrix44 matTemp = matJoint;
				if( animationListId.isValid() && this->mAnimationListDatas.Contains(animationListId))
				{					
					matTemp = this->mAnimationListDatas[animationListId][keyIndex];
				}

				Math::vector outTranslate;Math::quaternion outRotate;Math::vector outScale;
				matTemp.decompose(outScale,outRotate,outTranslate);
				curveTranslation.SetKey(keyIndex,outTranslate);
				curveRotation.SetKey(keyIndex, Math::float4(outRotate.x(),outRotate.y(),outRotate.z(),outRotate.w()));
				curveScale.SetKey(keyIndex, outScale);
			}

			clip.AddCurve(curveTranslation);
			clip.AddCurve(curveRotation);
			clip.AddCurve(curveScale);	
		}				

		clip.SetName("animatior_clip");
		clip.SetPreInfinityType( CoreAnimation::InfinityType::Cycle);
		clip.SetPostInfinityType( CoreAnimation::InfinityType::Cycle);
		clip.SetNumKeys(keyCount);
		clip.SetKeyStride(clip.GetNumCurves());
		clip.SetStartKeyIndex(0);
		///TODO:
		clip.SetKeyDuration(Timing::SecondsToTicks(0.04f));
		animBuilder.AddClip(clip);
		
		if( !AnimBuilderSaver::SaveNax3(animFilename,animBuilder,Platform::Win32) )
		{
			n_error("SaveNax3 Save [%s] Failed!\n",animFilename);
		}///end if	
		return animFilename;
	}
}