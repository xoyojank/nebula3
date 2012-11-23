//------------------------------------------------------------------------------
//  DAE2Nebula3Writer_Controller.cpp
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
	void Nebula3Writer::importSkinController(const COLLADAFW::Controller* Controller)
	{
		// Get the skin controller object.
		const COLLADAFW::UniqueId& controllerId = Controller->getUniqueId ();
		if( mCharacterDatas.Contains(controllerId)) return;
		COLLADAFW::SkinController* skinController = (COLLADAFW::SkinController*)Controller;

		mCharacterDatas.Add(controllerId,CharacterData());
		CharacterData& characterData		= this->mCharacterDatas[controllerId];
		characterData.controllerId			= controllerId;
		characterData.geometryUniqueId		= skinController->getSource ();
		characterData.skinUniqueId			= skinController->getSkinControllerData();

		/// 得到joint id集合		
		const COLLADAFW::UniqueIdArray& joints = skinController->getJoints ();
		for ( size_t i=0; i<joints.getCount (); ++i )
		{
			const COLLADAFW::UniqueId& jointId = joints [i];
			characterData.jointUniqueIds.Append(jointId);
		}///end for
	}

	//--------------------------------------------------------------------
	void Nebula3Writer::importSkinControllerData( const COLLADAFW::SkinControllerData* skinControllerData )
	{
		const COLLADAFW::UniqueId& skinControllerDataId = skinControllerData->getUniqueId ();	
		if( mSkinDatas.Contains(skinControllerDataId) ) return;

		this->mSkinDatas.Add(skinControllerDataId,SkinData());
		SkinData& skinData = this->mSkinDatas[skinControllerDataId];
		
		size_t jointCount = skinControllerData->getJointsCount();

		size_t size = skinControllerData->getVertexCount();	
		const UIntValuesArray& jointNumPerVertex = skinControllerData->getJointsPerVertex();
		const UIntValuesArray& weightIndices = skinControllerData->getWeightIndices();
		const IntValuesArray& jointIndices = skinControllerData->getJointIndices();		
		const COLLADAFW::ArrayPrimitiveType<float>* weights = skinControllerData->getWeights().getFloatValues ();

		/// 骨骼权重信息	
		int countBase =0;
		for (unsigned int index=0; index < size;index++ )
		{
			int count = jointNumPerVertex[index];
			JointIndex jointIndex; Weight weightDatas;			
			for (int j = 0; j< count && j < 4;j++)
			{
				IndexT boneIndex = jointIndices[countBase + j];
				IndexT weightIndex = weightIndices[countBase +j];
				jointIndex.Indices[j] =boneIndex;
				weightDatas.Weights[j] = weights->getData()[weightIndex];
			}
			countBase += count;
			skinData.jointIndices.Append(jointIndex);
			skinData.weightIndices.Append(weightDatas);
		}/// end for	

		const Matrix4Array& inverseMatirxs = skinControllerData->getInverseBindMatrices();
		for (size_t index=0; index < inverseMatirxs.getCount();index++)
		{
			Math::matrix44 matrix4 = this->toMatrix44(inverseMatirxs[index]);
			skinData.inverseMatirxs.Append(matrix4);
		}

		/// 骨骼所对应的蒙皮的世界矩阵
		skinData.bindShapeMatrix = this->toMatrix44(skinControllerData->getBindShapeMatrix());		
	}

	//--------------------------------------------------------------------
	void Nebula3Writer::buildJoint(const NodeData& nodeData,CharacterData& characterData)
	{	
		JointMaps& characterJoints = characterData.characterJoints;
		n_printf("nodeName:%s\n",nodeData.nodeName.AsCharPtr());

		//	添加骨骼
		//  设置骨骼名称
		//  设置骨骼索引以及更新骨骼的父子关系
		if( nodeData.nodeType == NodeData::JOINT)
		{
			const COLLADAFW::UniqueId& uniqueId = nodeData.nodeUniqueId;
			characterJoints.Add(uniqueId,CharacterJoint());
			IndexT jointIndex = characterJoints.Size()-1;
			CharacterJoint& joint = characterJoints[uniqueId];
			joint.SetName(nodeData.nodeName);			
			joint.SetJointIndex( jointIndex );

			Math::matrix44 matTemp = Math::matrix44::inverse(characterData.inverseMatrixs[uniqueId]);
			if( uniqueId != characterData.rootUniqueId)
			{				
				NodeData parentNodeData = this->mNodeMaps[nodeData.parentUniqueId];
				while (	parentNodeData.nodeType != NodeData::JOINT )
				{
					//parentNodeData.parentUniqueId.isValid()
					parentNodeData = this->mNodeMaps[parentNodeData.parentUniqueId];
				}///end while

				const COLLADAFW::UniqueId parentJointId = parentNodeData.nodeUniqueId;
				if(characterData.characterJoints.Contains(parentJointId)) 
				{			
					const CharacterJoint& parentJoint = characterData.characterJoints[parentJointId];
					joint.SetParentJointIndex( parentJoint.GetJointIndex() );
					matTemp = Math::matrix44::multiply(matTemp,characterData.inverseMatrixs[parentJointId]);
				}
				else
				{
					n_warning("[%s] not parent Joint!\n",nodeData.nodeName.AsCharPtr());
				}
			}///end if
			else
			{
				const SkinData& skinData = this->mSkinDatas[characterData.skinUniqueId];
				matTemp = Math::matrix44::multiply(matTemp,characterData.RootTransform);
				matTemp = Math::matrix44::multiply(matTemp,Math::matrix44::inverse(skinData.bindShapeMatrix));
			}

			Math::vector outTranslate;Math::quaternion outRotate;Math::vector outScale;
			matTemp.decompose(outScale,outRotate,outTranslate);
			joint.SetPoseTranslation( outTranslate );
			joint.SetPoseRotation( outRotate );
			joint.SetPoseScale( outScale );
			characterData.jointMatrix.Add(uniqueId,matTemp);
		}

		/// 遍历子骨骼
		for (int i=0; i< nodeData.childUniqueIds.Size();++i)
		{
			const COLLADAFW::UniqueId& childId = nodeData.childUniqueIds[i];
			const NodeData& tempData = this->mNodeMaps[childId];
			this->buildJoint(tempData,characterData);
		}///end for
	}

	//--------------------------------------------------------------------
	void Nebula3Writer::buildSkeleton(CharacterData& characterData,const SkinData& skinData )
	{
		/// 骨骼中合并骨骼的逆矩阵
		for (int i=0;i< characterData.jointUniqueIds.Size();i++)
		{
			const COLLADAFW::UniqueId& jointId = characterData.jointUniqueIds[i];
			Math::matrix44& inverseMatirx = skinData.inverseMatirxs[i];
			characterData.inverseMatrixs.Add(jointId,inverseMatirx);
		}
		/// 从骨骼根节点进行遍历
		const NodeData& nodeData = this->mNodeMaps[characterData.rootUniqueId];
		this->buildJoint(nodeData,characterData);

		for (int i=0;i< characterData.jointUniqueIds.Size();i++)
		{
			const COLLADAFW::UniqueId& jointId = characterData.jointUniqueIds[i];
			Math::matrix44& inverseMatirx = skinData.inverseMatirxs[i];
			characterData.inverseMatrixs.Add(jointId,inverseMatirx);
		}
	}

	//--------------------------------------------------------------------
	Ptr<TransformNode> Nebula3Writer::writeSkinGeometryData( const NodeData& instanceNodeData,const SkinData& skinData,GeometryData& geometryData )
	{
		Ptr<TransformNode> transformNode = TransformNode::Create();
		MeshBuilder meshBuilder;
		Util::String meshFilename ="msh:" + geometryData.mGeometryName;meshFilename.Append(".nvx2");
		for (int groupIndex=0;groupIndex<geometryData.mMeshDatas.Size();groupIndex++)
		{
			/// 设置基本信息
			GeometryData::MeshData& meshData = geometryData.mMeshDatas[groupIndex];
			Ptr<CharacterSkinNode> shapeNode = CharacterSkinNode::Create();
			Util::String shapeName(geometryData.mGeometryName);
			shapeName.Append("_s_");
			shapeName.AppendInt(groupIndex);
			shapeNode->SetName(shapeName);
			shapeNode->SetPrimitiveGroupIndex(groupIndex);
			shapeNode->SetMeshResourceId(meshFilename);	
			this->writeMaterialData(instanceNodeData,geometryData,groupIndex,shapeNode.upcast<ModelNode>());

			for (int j=0;j< meshData.positionIndices.Size();j+=3 )
			{
				MeshBuilderVertex vertex01,vertex02,vertex03;
				vertex01.SetComponent(MeshBuilderVertex::CoordIndex,geometryData.mPositions[meshData.positionIndices[j]]);
				vertex02.SetComponent(MeshBuilderVertex::CoordIndex,geometryData.mPositions[meshData.positionIndices[j+1]]);
				vertex03.SetComponent(MeshBuilderVertex::CoordIndex,geometryData.mPositions[meshData.positionIndices[j+2]]);

				if( !meshData.normalIndices.IsEmpty())
				{
					vertex01.SetComponent(MeshBuilderVertex::NormalIndex,geometryData.mNormals[meshData.normalIndices[j]]);
					vertex02.SetComponent(MeshBuilderVertex::NormalIndex,geometryData.mNormals[meshData.normalIndices[j+1]]);
					vertex03.SetComponent(MeshBuilderVertex::NormalIndex,geometryData.mNormals[meshData.normalIndices[j+2]]);
				}
				if( !meshData.mDiffMapUVs.IsEmpty())
				{
					vertex01.SetComponent(MeshBuilderVertex::Uv0Index,meshData.mDiffMapUVs[meshData.mDiffMapIndices[j]]);
					vertex02.SetComponent(MeshBuilderVertex::Uv0Index,meshData.mDiffMapUVs[meshData.mDiffMapIndices[j+1]]);
					vertex03.SetComponent(MeshBuilderVertex::Uv0Index,meshData.mDiffMapUVs[meshData.mDiffMapIndices[j+2]]);
				}

				if( !geometryData.mJointIndices.IsEmpty())
				{
					vertex01.SetComponent(MeshBuilderVertex::JIndicesIndex,geometryData.mJointIndices[meshData.positionIndices[j]]);
					vertex01.SetComponent(MeshBuilderVertex::WeightsIndex,geometryData.mWeights[meshData.positionIndices[j]]);
					vertex02.SetComponent(MeshBuilderVertex::JIndicesIndex,geometryData.mJointIndices[meshData.positionIndices[j+1]]);
					vertex02.SetComponent(MeshBuilderVertex::WeightsIndex,geometryData.mWeights[meshData.positionIndices[j+1]]);
					vertex03.SetComponent(MeshBuilderVertex::JIndicesIndex,geometryData.mJointIndices[meshData.positionIndices[j+2]]);
					vertex03.SetComponent(MeshBuilderVertex::WeightsIndex,geometryData.mWeights[meshData.positionIndices[j+2]]);
				}				

				meshBuilder.AddVertex(vertex01);
				size_t indices01 = meshBuilder.GetNumVertices() -1;
				meshBuilder.AddVertex(vertex02);
				size_t indices02 = meshBuilder.GetNumVertices() -1;
				meshBuilder.AddVertex(vertex03);
				size_t indices03 = meshBuilder.GetNumVertices() -1;

				MeshBuilderTriangle triangle;
				triangle.SetGroupId(groupIndex);
				triangle.SetVertexIndices(indices01,indices02,indices03);
				meshBuilder.AddTriangle(triangle);				
			}///end for	
			transformNode->AddChild(shapeNode.upcast<ModelNode>());
		}///end for

		/// 保存模型文件		
		if( !MeshBuilderSaver::SaveNvx2(meshFilename,meshBuilder,Platform::Win32) )
		{
			n_error("Save [%s] Failed!\n",meshFilename);
		}///end if	

		Math::bbox boundingbox = meshBuilder.ComputeBoundingBox();
		transformNode->SetName(geometryData.mGeometryName);
		transformNode->SetBoundingBox(boundingbox);
		Util::Array<Ptr<ModelNode>> shapeNodes = transformNode->GetChildren();
		for (int i=0;i<shapeNodes.Size();i++)
		{
			shapeNodes[i]->SetBoundingBox(boundingbox);
		}///end for

		/// 设置位置信息
		//Math::vector outTranslate;Math::quaternion outRotate;Math::vector outScale;		
		//Math::matrix44 matTemp = this->getGeometryMatrix(instanceNodeData.nodeUniqueId);
		//matTemp.decompose(outScale,outRotate,outTranslate);	
		//transformNode->SetPosition(outTranslate);
		//transformNode->SetRotation(outRotate);
		//transformNode->SetScale(outScale);	
		return transformNode;
	}

	//--------------------------------------------------------------------
	void Nebula3Writer::buildSkinWeights(const CharacterData& characterData,const SkinData& skinData,GeometryData& geometryData )
	{
		Util::Dictionary<IndexT,IndexT> converterIndices;
		for (int i=0;i< characterData.jointUniqueIds.Size();i++)
		{
			const COLLADAFW::UniqueId& jointId = characterData.jointUniqueIds[i];
			const NodeData& nodeData = this->mNodeMaps[jointId];
			const CharacterJoint& joint = characterData.characterJoints[jointId];
			converterIndices.Add(i,joint.GetJointIndex());
		}

		/// 建立蒙皮信息
		for (int i=0;i<skinData.jointIndices.Size();i++)
		{
			JointIndex& jointIndex = skinData.jointIndices[i];
			Weight& weight = skinData.weightIndices[i];
			IndexT x=0,y=0,z=0,w=0;

			if( weight.Weights[0]!=0.0f)
				x = converterIndices[jointIndex.Indices[0]];
			if( weight.Weights[1]!=0.0f)
				y = converterIndices[jointIndex.Indices[1]];
			if( weight.Weights[2]!=0.0f)
				z = converterIndices[jointIndex.Indices[2]];
			if( weight.Weights[3]!=0.0f)
				w = converterIndices[jointIndex.Indices[3]];

			Math::float4 boneIndices((float)x,(float)y,(float)z,(float)w);
			Math::float4 boneWeight(weight.Weights[0],weight.Weights[1],weight.Weights[2],weight.Weights[3]);
			geometryData.mJointIndices.Append(boneIndices);
			geometryData.mWeights.Append(boneWeight);
		}///end for
	}

	//--------------------------------------------------------------------
	Ptr<CharacterNode> Nebula3Writer::writeCharacterData( const NodeData& instanceNodeData,CharacterData& characterData )
	{
		if( !this->mGeometryDatas.Contains(characterData.geometryUniqueId) ||
			!this->mSkinDatas.Contains(characterData.skinUniqueId) )
		{
			n_error("CharacterData Not have geometryData or SkinData!\n");
		}

		GeometryData& geometryData = this->mGeometryDatas[characterData.geometryUniqueId];
		SkinData& skinData = this->mSkinDatas[characterData.skinUniqueId];
		/// 设置骨架基本信息		
		characterData.RootTransform = instanceNodeData.bind_matrix;
		characterData.characterName = instanceNodeData.nodeName;

		/// 建立骨骼
		this->buildSkeleton(characterData,skinData);
		this->buildSkinWeights(characterData,skinData,geometryData);		
		
		/// 建立骨骼节点
		Ptr<CharacterNode> characterNode = CharacterNode::Create();
		characterNode->SetName(characterData.characterName);

		/// 为骨骼节点添加骨骼
		Util::Array<IndexT> boneIndices;
		const JointMaps& characterJoints = characterData.characterJoints;
		const Util::Array<COLLADAFW::UniqueId>& jointKeys = characterJoints.KeysAsArray();
		for (int i=0;i<jointKeys.Size();i++)
		{
			const COLLADAFW::UniqueId& jointId = jointKeys[i];
			characterNode->AddJoint( jointId,characterJoints[jointId] );
			//if( characterData.inverseMatrixs.Contains(jointId))
			boneIndices.Append( characterJoints[jointId].GetJointIndex());
		}///end for

		/// 为骨骼节点添加蒙皮信息
		Ptr<TransformNode> transformNode = this->writeSkinGeometryData(instanceNodeData,skinData,geometryData);
		Util::Array<Ptr<ModelNode>> shapeNodes = transformNode->GetChildren();
		for (int i=0;i< shapeNodes.Size();i++)
		{
			const Ptr<CharacterSkinNode>& skinNode = shapeNodes[i].downcast<CharacterSkinNode>();
			IndexT groupIndex = skinNode->GetPrimitiveGroupIndex();
			skinNode->AddFragment(groupIndex,boneIndices);					
		}///end for

		Util::String animResId = this->writeAnimationData(characterData,characterJoints);
		characterNode->SetAnimationResourceId(animResId);
		characterNode->AddChild(transformNode.upcast<ModelNode>());
		characterNode->AddSkin(transformNode->GetName().AsString());	

		//Math::vector outTranslate;Math::quaternion outRotate;Math::vector outScale;		
		//Math::matrix44 matTemp = characterData.RootTransform;
		//matTemp.decompose(outScale,outRotate,outTranslate);	
		//characterNode->SetPosition(outTranslate);
		//characterNode->SetRotation(outRotate);
		//characterNode->SetScale(outScale);
		return characterNode;
	}
}