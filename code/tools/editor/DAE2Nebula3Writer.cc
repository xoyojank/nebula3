//------------------------------------------------------------------------------
//  DAE2NebulaWriter.cc
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
	Nebula3Writer::Nebula3Writer( const COLLADABU::URI& inputFile, const COLLADABU::URI& outputFile )
		: mInputFile(inputFile)
		, mOutputFile(outputFile)
	{
	}

	//--------------------------------------------------------------------
	Nebula3Writer::~Nebula3Writer()
	{		
	}

	//--------------------------------------------------------------------
	bool Nebula3Writer::write()
	{
		COLLADASaxFWL::Loader loader;
		COLLADAFW::Root root(&loader, this);

		// Load scene graph 
		if ( !root.loadDocument(mInputFile.toNativePath()) )
			return false;
		return true;
	}

	//--------------------------------------------------------------------
	void Nebula3Writer::cancel( const String& errorMessage )
	{
	}

	//--------------------------------------------------------------------
	void Nebula3Writer::start()
	{
	}

	//--------------------------------------------------------------------
	void Nebula3Writer::finish()
	{
		this->makeDirectXFriendly();
		Util::String modelFilename = MODEL_PRE_FIXED;
		IoServer::Instance()->CreateDirectory(URI(modelFilename));
		modelFilename.Append(mOutputFile.getPathFile().c_str());	
		this->writeModel(modelFilename);
	}

	//--------------------------------------------------------------------
	bool Nebula3Writer::isCoords()
	{
		return this->mUpAxisType == COLLADAFW::FileInfo::Y_UP;
	}

	//--------------------------------------------------------------------
	bool Nebula3Writer::writeGlobalAsset( const COLLADAFW::FileInfo* asset )
	{
		//n_printf("writeGlobalAsset......\n");
		const COLLADAFW::FileInfo::Unit& unit = asset->getUnit ();
		mUpAxisType = asset->getUpAxisType ();
		mUnit       = unit.getLinearUnitUnit ();
		return true;
	}

	//--------------------------------------------------------------------
	bool Nebula3Writer::writeVisualScene( const COLLADAFW::VisualScene* visualScene )
	{
		this->importVisualScene(visualScene);
		return true;
	}

	//--------------------------------------------------------------------
	bool Nebula3Writer::writeScene( const COLLADAFW::Scene* scene )
	{
		//n_printf("writeScene......\n");
		return true;
	}

	//--------------------------------------------------------------------
	bool Nebula3Writer::writeLibraryNodes( const COLLADAFW::LibraryNodes* libraryNodes )
	{
		//n_printf("writeLibraryNodes......\n");
		return true;
	}

	//--------------------------------------------------------------------
	bool Nebula3Writer::writeMaterial( const COLLADAFW::Material* material )
	{
		// Check if the current material is already imported.
		const COLLADAFW::UniqueId& materialId = material->getUniqueId ();
		const COLLADAFW::UniqueId& effectId = material->getInstantiatedEffect();
		this->mMaterialDatas.Add(materialId,effectId);
		return true;
	}

	//--------------------------------------------------------------------
	bool Nebula3Writer::writeEffect( const COLLADAFW::Effect* effect )
	{
		// Check if the current effect is already imported.
		const COLLADAFW::UniqueId& effectId = effect->getUniqueId ();
		const COLLADAFW::CommonEffectPointerArray& commonEffects = effect->getCommonEffects ();
		 size_t numCommonEffects = commonEffects.getCount ();
		 if( numCommonEffects !=1) return false;
		COLLADAFW::EffectCommon* commonEffect = commonEffects [0];
		const COLLADAFW::EffectCommon::ShaderType& shaderType = commonEffect->getShaderType ();
		EffectData effectData;
		switch ( shaderType )
		{
		case COLLADAFW::EffectCommon::SHADER_BLINN:
		case COLLADAFW::EffectCommon::SHADER_CONSTANT:
		case COLLADAFW::EffectCommon::SHADER_PHONG:
		case COLLADAFW::EffectCommon::SHADER_LAMBERT:
			{
				const COLLADAFW::ColorOrTexture& diffuse = commonEffect->getDiffuse ();
				if ( diffuse.isTexture () )
				{
					// Get the texure and the current shader attribute.
					const COLLADAFW::Texture& texture = diffuse.getTexture ();
					effectData.diffMapId = texture.getTextureMapId();
					effectData.diffSamplerId = texture.getSamplerId();
				}

				const COLLADAFW::ColorOrTexture& emission = commonEffect->getEmission ();
				if ( emission.isTexture () )
				{
					const COLLADAFW::Texture& texture = emission.getTexture ();
					effectData.emsvMapId = texture.getTextureMapId();
					effectData.emsvSamplerId = texture.getSamplerId();
				}

				const COLLADAFW::ColorOrTexture& specular = commonEffect->getSpecular();
				if ( specular.isTexture () )
				{
					const COLLADAFW::Texture& texture = specular.getTexture ();
					effectData.specMapId = texture.getTextureMapId();
					effectData.specSamplerId = texture.getSamplerId();
				}
			}
			break;
		case COLLADAFW::EffectCommon::SHADER_UNKNOWN:
		default:
			// Import as a lambert shader.
			n_warning("Unknown shader type!\n");
			break;
		}

		const COLLADAFW::SamplerPointerArray& samplers = commonEffect->getSamplerPointerArray ();
		size_t numSamplers = samplers.getCount ();
		for ( size_t samplerId=0; samplerId<numSamplers; ++samplerId )
		{
			COLLADAFW::Sampler* sampler = samplers [samplerId];
			const COLLADAFW::UniqueId& imageId = sampler->getSourceImage ();
			COLLADAFW::Sampler::SamplerType samplerType = sampler->getSamplerType ();
			if( samplerType !=COLLADAFW::Sampler::SAMPLER_TYPE_2D ) continue;

			if( effectData.diffSamplerId == samplerId)
				effectData.diffImageUniqueId = imageId;

			if( effectData.emsvSamplerId == samplerId)
				effectData.emsvImageUniqueId = imageId;

			if( effectData.emsvSamplerId == samplerId)
				effectData.specImageUniqueId = imageId;

			if( effectData.bumpSamplerId == samplerId)
				effectData.bumpImageUniqueId = imageId;
		}

		mEffectDatas.Add(effectId,effectData);
		return true;
	}

	//--------------------------------------------------------------------
	bool Nebula3Writer::writeCamera( const COLLADAFW::Camera* camera )
	{
		//n_printf("writeCamera......\n");
		return true;
	}

	//--------------------------------------------------------------------
	bool Nebula3Writer::writeImage( const COLLADAFW::Image* image )
	{
		const COLLADAFW::UniqueId& imageId = image->getUniqueId ();
		const COLLADABU::URI& imageURI = image->getImageURI ();
		Util::String fileTextureName(imageURI.toNativePath ().c_str());
		//fileTextureName = fileTextureName.ExtractFileName();

		this->mImageDatas.Add(imageId,fileTextureName);
		return true;
	}

	//--------------------------------------------------------------------
	bool Nebula3Writer::writeTextureData(const COLLADAFW::UniqueId& imageId,Util::String& newTextureName)
	{
		static const Util::String newTextureDir = TEXTURE_PRE_FIXED;
		if( !IoServer::Instance()->DirectoryExists(URI(newTextureDir)))
			IoServer::Instance()->CreateDirectory( URI(newTextureDir));		
		Util::String fileTextureName = this->mImageDatas[imageId];		
		if( IoServer::Instance()->FileExists(URI(fileTextureName)))
		{
			newTextureName = newTextureDir;
			newTextureName.Append(fileTextureName.ExtractFileName());
			if( !IoServer::Instance()->FileExists(URI(newTextureName)))
				IoServer::Instance()->CopyFile(URI(fileTextureName),URI(newTextureName));
		}
		else
		{
			n_warning("texture not exist [%s]\n",fileTextureName.AsCharPtr());
			return false;
		}	

		return true;
	}

	//--------------------------------------------------------------------
	bool Nebula3Writer::writeMaterialData( const NodeData& instanceNodeData,GeometryData& geometryData,IndexT groupIndex,const Ptr<ModelNode>& modelNode )
	{		
		GeometryData::MeshData& meshData = geometryData.mMeshDatas[groupIndex];
		const COLLADAFW::MaterialId& materialId = meshData.materialId;
		const Ptr<StateNode>& shapeNode = modelNode.downcast<StateNode>();
		bool useDefaultMaterial = true;
		
		/// 找不到材质
		if( instanceNodeData.materialDatas.Contains(materialId) ) 
		{
			const MaterialData& materialData = instanceNodeData.materialDatas[materialId];
			if( this->mMaterialDatas.Contains(materialData.materialUniqueId) )
			{
				const COLLADAFW::UniqueId& effectId = this->mMaterialDatas[materialData.materialUniqueId];
				if( this->mEffectDatas.Contains(effectId) )
				{
					const EffectData& effectData = this->mEffectDatas[effectId];
					if( materialData.textureDatas.Contains(effectData.diffMapId) )
					{
						/// 找到diffuse贴图坐标和贴图纹理
						const MaterialData::TextureCoordData& textureData = materialData.textureDatas[effectData.diffMapId];
						GeometryData::TextureData& uvData = meshData.uvDatas[textureData.uvSetIndex];
						const Util::Array<Math::vector>& uvs = geometryData.mUvs[uvData.uvSetname];	
						const Util::Array<IndexT>& uvIndices = uvData.uvIndices;
						n_assert(meshData.mDiffMapIndices.IsEmpty());
						n_assert(meshData.mDiffMapUVs.IsEmpty());
						meshData.mDiffMapUVs.AppendArray(uvs);
						meshData.mDiffMapIndices.AppendArray(uvIndices);
						Util::String newTexturename;
						if( effectData.diffImageUniqueId.isValid() && this->writeTextureData(effectData.diffImageUniqueId,newTexturename))
							meshData.mDiffTextureName = newTexturename;
						if( effectData.emsvImageUniqueId.isValid() && this->writeTextureData(effectData.emsvImageUniqueId,newTexturename))
							meshData.mEmsvTextureName = newTexturename;
						if( effectData.specImageUniqueId.isValid() && this->writeTextureData(effectData.specImageUniqueId,newTexturename))
							meshData.mSpecTextureName = newTexturename;
						if( effectData.bumpImageUniqueId.isValid() && this->writeTextureData(effectData.bumpImageUniqueId,newTexturename))
							meshData.mBumpTextureName = newTexturename;
						useDefaultMaterial = false;
					}///end if
				}///end if
			}///end if
		}

		/// 默认材质
		shapeNode->SetShader("shd:static");	
		shapeNode->AddShaderParam("DiffMap0",meshData.mDiffTextureName);
		shapeNode->AddShaderParam("EmsvMap0",meshData.mEmsvTextureName);
		shapeNode->AddShaderParam("SpecMap0",meshData.mSpecTextureName);
		shapeNode->AddShaderParam("BumpMap0",meshData.mBumpTextureName);		
		return useDefaultMaterial;
	}

	//--------------------------------------------------------------------
	bool Nebula3Writer::writeLight( const COLLADAFW::Light* light )
	{
		//n_printf("writeLight......\n");
		return true;
	}

	//--------------------------------------------------------------------
	bool Nebula3Writer::writeAnimation( const COLLADAFW::Animation* animation )
	{		
		const COLLADAFW::UniqueId& animationId = animation->getUniqueId ();
		if( mAnimationDatas.Contains(animationId) ) return false;
		
		// Import the animation data.
		const COLLADAFW::Animation::AnimationType& animationType = animation->getAnimationType ();
		switch ( animationType )
		{
		case COLLADAFW::Animation::ANIMATION_CURVE:
			{
				AnimationData animData;
				COLLADAFW::AnimationCurve* animationCurve = ( COLLADAFW::AnimationCurve* ) animation;
				importAnimationCurve ( animationCurve ,animData);
				mAnimationDatas.Add(animationId,animData);
				break;
			}
		case COLLADAFW::Animation::ANIMATION_FORMULA:
			break;
		default:
			break;
		}

		
		return true;
	}

	//--------------------------------------------------------------------
	bool Nebula3Writer::writeAnimationList( const COLLADAFW::AnimationList* animationList )
	{	
		this->importAnimationList(animationList);
		return true;
	}

	//--------------------------------------------------------------------
	bool Nebula3Writer::writeSkinControllerData( const COLLADAFW::SkinControllerData* skinControllerData )
	{
		this->importSkinControllerData(skinControllerData);
		return true;
	}

	//--------------------------------------------------------------------
	bool Nebula3Writer::writeController( const COLLADAFW::Controller* Controller )
	{
		const COLLADAFW::UniqueId& controllerId = Controller->getUniqueId ();
		COLLADAFW::Controller::ControllerType controllerType = Controller->getControllerType ();
		switch ( controllerType )
		{
		case COLLADAFW::Controller::CONTROLLER_TYPE_SKIN:
			this->importSkinController(Controller);
			break;
		case COLLADAFW::Controller::CONTROLLER_TYPE_MORPH:
		default:
			std::cerr << "Unknown controller type!" << std::endl;
			break;
		}
		return true;
	}

	//--------------------------------------------------------------------
	Math::matrix44 Nebula3Writer::toMatrix44(const COLLADABU::Math::Matrix4& matMatrix)
	{
		Math::matrix44 matTemp;
		matTemp.setrow0(Math::float4((float)matMatrix.getElement(0,0),(float)matMatrix.getElement(0,1),(float)matMatrix.getElement(0,2),(float)matMatrix.getElement(0,3)));
		matTemp.setrow1(Math::float4((float)matMatrix.getElement(1,0),(float)matMatrix.getElement(1,1),(float)matMatrix.getElement(1,2),(float)matMatrix.getElement(1,3)));
		matTemp.setrow2(Math::float4((float)matMatrix.getElement(2,0),(float)matMatrix.getElement(2,1),(float)matMatrix.getElement(2,2),(float)matMatrix.getElement(2,3)));
		matTemp.setrow3(Math::float4((float)matMatrix.getElement(3,0),(float)matMatrix.getElement(3,1),(float)matMatrix.getElement(3,2),(float)matMatrix.getElement(3,3)));
		return matTemp;
	}

	//--------------------------------------------------------------------
	Math::matrix44 Nebula3Writer::toDirectXMatrix(const Math::matrix44& matrix44)
	{
		//Math::vector outTranslate;Math::quaternion outRotate;Math::vector outScale;
		//Math::vector outAxis;Math::scalar outAngle;
		Math::matrix44 matTemp = Math::matrix44::transpose(matrix44);
		//matTemp.decompose(outScale,outRotate,outTranslate);
		//matTemp = Math::matrix44::scaling(outScale.x(),outScale.z(),outScale.y());
		//Math::quaternion::to_axisangle(outRotate,outAxis,outAngle);
		//if( outAxis.y()!=0.0f || outAxis.z()!=0.0f)
		//{
		//	outRotate = Math::quaternion::rotationaxis(Math::vector(outAxis.x(),outAxis.z(),outAxis.y()),-outAngle);
		//}
		//matTemp = Math::matrix44::multiply(matTemp,Math::matrix44::rotationquaternion(outRotate));
		//matTemp = Math::matrix44::multiply(matTemp,Math::matrix44::translation(outTranslate.x(),outTranslate.z(),outTranslate.y()));
		return matTemp;
	}

	//--------------------------------------------------------------------
	void Nebula3Writer::makeDirectXFriendly()
	{
		Util::Array<COLLADAFW::UniqueId> keys = this->mNodeMaps.KeysAsArray();
		for (int i=0;i< keys.Size();i++)
		{
			NodeData& nodeData =this->mNodeMaps[keys[i]];
			nodeData.bind_matrix = this->toDirectXMatrix(nodeData.bind_matrix);
		}///end for

		keys = this->mSkinDatas.KeysAsArray();
		for (int i=0;i< keys.Size();i++ )
		{
			SkinData& skinData = this->mSkinDatas[keys[i]];		
			skinData.bindShapeMatrix = this->toDirectXMatrix(skinData.bindShapeMatrix);

			for (int j=0;j<skinData.inverseMatirxs.Size();j++)
			{				
				skinData.inverseMatirxs[j] = this->toDirectXMatrix(skinData.inverseMatirxs[j]);
			}///end for
		}///end for

		keys = this->mAnimationListDatas.KeysAsArray();
		for (int i=0;i< keys.Size();i++)
		{
			Util::Array<Math::matrix44>& animationKeys = this->mAnimationListDatas[keys[i]];
			for (int j=0;j<animationKeys.Size();j++)
			{
				animationKeys[j] = this->toDirectXMatrix(animationKeys[j]);
			}///end for
		}///end for
		
	}	

	//--------------------------------------------------------------------
	void Nebula3Writer::writeModel(Util::String& modelFilename)
	{
		Ptr<Model> model = Model::Create();		
		if( !this->mControllerNodeMaps.IsEmpty())
		{
			const Util::Array<COLLADAFW::UniqueId>& keys = this->mControllerNodeMaps.KeysAsArray();
			const COLLADAFW::UniqueId controllerUniqueId = keys[0];
			if( this->mCharacterDatas.Contains(controllerUniqueId))
			{
				const COLLADAFW::UniqueId& instanceNodeId  = this->mControllerNodeMaps[controllerUniqueId];
				if( this->mNodeMaps.Contains(instanceNodeId))
				{
					CharacterData& characterData = this->mCharacterDatas[controllerUniqueId];
					const NodeData& instanceNodeData = this->mNodeMaps[instanceNodeId];
					Ptr<CharacterNode> characterNode = this->writeCharacterData(instanceNodeData,characterData);
					model->AttachNode(characterNode.upcast<ModelNode>());
				}				
			}///end if
		}
		else if( !this->mGeometryNodeMaps.IsEmpty())
		{			
			Ptr<TransformNode> rootNode = TransformNode::Create();	
			const Util::Array<COLLADAFW::UniqueId>& keys = this->mGeometryNodeMaps.KeysAsArray();
			for (int i=0;i< keys.Size();i++ )
			{
				const COLLADAFW::UniqueId geometryUniqueId = keys[i];
				if( !this->mGeometryDatas.Contains(geometryUniqueId)) continue;
				const COLLADAFW::UniqueId& instanceNodeId  = this->mGeometryNodeMaps[geometryUniqueId];
				if( !this->mNodeMaps.Contains(instanceNodeId)) continue;
						
				GeometryData& geometryData = this->mGeometryDatas[geometryUniqueId];
				const NodeData& instanceNodeData = this->mNodeMaps[instanceNodeId];
				Ptr<TransformNode> transformNode = this->writeGeometryData(instanceNodeData,geometryData);				
				rootNode->AddChild(transformNode.upcast<ModelNode>());
			}///end for

			rootNode->SetName("model");
			model->AttachNode(rootNode.upcast<ModelNode>());
		}
		else
		{
			n_printf("no model export!");
			return;
		}

		Ptr<Stream> stream = IoServer::Instance()->CreateStream(URI(modelFilename));
		Ptr<Editor::ModelNodeWriter> writer = Editor::ModelNodeWriter::Create();
		writer->SetStream(stream);
		if (writer->Open())
		{
			writer->WriteModel("model",model);
			writer->Close();
		}
		model = 0;
		writer = 0;
	}

} // namespace DAE2Ogre
