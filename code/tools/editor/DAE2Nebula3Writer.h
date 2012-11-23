/*
Copyright (C) 2010 Dengwenyi

This file is part of dae2n3.

Licensed under the MIT Open Source License
*/

#ifndef __DAE2N3_NEBULA3WRITER_H__
#define __DAE2N3_NEBULA3WRITER_H__

#include "COLLADAFWIWriter.h"
#include "COLLADAFWUniqueId.h"
#include "COLLADAFWVisualScene.h"
#include "COLLADAFWLibraryNodes.h"
#include "COLLADAFWMaterial.h"
#include "COLLADAFWEffect.h"
#include "COLLADAFWImage.h"
#include "COLLADAFWAnimationCurve.h"
#include "COLLADAFWMesh.h"
#include "COLLADAFWFileInfo.h"

#include "COLLADABUURI.h"
#include "Math/COLLADABUMathMatrix4.h"
#include "toolkitutil/meshutil/meshbuildersaver.h"
#include "toolkitutil/animutil/animbuildersaver.h"

#include <util/array.h>
#include <util/String.h>
#include <util/dictionary.h>
#include "models/nodes/shapenode.h"
#include "models/nodes/transformnode.h"
#include "models/nodes/animatornode.h"
#include "characters/characterskinnode.h"
#include "characters/characternode.h"
#include "Index.h"
#include "Weight.h"

#define MODEL_PRE_FIXED "mdl:"
#define TEXTURE_PRE_FIXED "tex:"
#define MESH_PRE_FIXED "msh:"
#define ANIM_PRE_FIXED "ani:"

namespace Models
{
	class SkinData
	{
	public:
		Util::Array<JointIndex>		jointIndices;
		Util::Array<Weight>		weightIndices;
		Util::Array<Math::matrix44> inverseMatirxs;///fixed
		Math::matrix44				bindShapeMatrix;///fixed

		SkinData():bindShapeMatrix(Math::matrix44::identity()){}
	};

	class GeometryData
	{
	public:		
		struct TextureData
		{
			IndexT uvSetIndex;
			Util::String uvSetname;			
			SizeT stride;
			IndexT baseIndex;
			Util::Array<IndexT> uvIndices;
		};

		struct MeshData
		{
			Util::Array<IndexT> positionIndices;
			Util::Array<IndexT> normalIndices;

			///[uvSetIndex,TextureData]
			Util::Dictionary<IndexT,TextureData> uvDatas;

			/// 对应的材质ID
			COLLADAFW::MaterialId materialId;
			Util::String materialName;

			Util::Array<Math::vector>		mDiffMapUVs;
			Util::Array<IndexT>				mDiffMapIndices;
			Util::String					mDiffTextureName;
			Util::String					mEmsvTextureName;
			Util::String					mSpecTextureName;
			Util::String					mBumpTextureName;

			MeshData()
				:materialName("defaultMaterial")
				,mDiffTextureName("tex:system/check")
				,mEmsvTextureName("tex:system/black")
				,mSpecTextureName("tex:system/white")
				,mBumpTextureName("tex:system/nobump")
			{
			}
		};

		COLLADAFW::UniqueId				mGeometryUniqueId;
		Util::String					mGeometryName;
		Util::Array<Math::vector>		mPositions;
		Util::Array<Math::vector>		mNormals;		
		Util::Dictionary<Util::String,Util::Array<Math::vector>> mUvs;
		Util::Array<Math::float4>		mJointIndices;
		Util::Array<Math::float4>		mWeights;
		Util::Array<MeshData> mMeshDatas;
	};

	class CharacterData
	{
	public:
		COLLADAFW::UniqueId controllerId;
		Util::String characterName;
		COLLADAFW::UniqueId rootUniqueId;
		COLLADAFW::UniqueId geometryUniqueId;
		COLLADAFW::UniqueId skinUniqueId;
		Util::Array<COLLADAFW::UniqueId> jointUniqueIds;

		Util::Dictionary<COLLADAFW::UniqueId,Math::matrix44> inverseMatrixs;
		//Util::Dictionary<COLLADAFW::UniqueId,Math::matrix44> bindMatrixs;
		Math::matrix44 RootTransform;
		//Util::Array<Characters::CharacterJoint> characterJoints;
		Util::Dictionary<COLLADAFW::UniqueId,Characters::CharacterJoint> characterJoints;
		Util::Dictionary<COLLADAFW::UniqueId,Math::matrix44> jointMatrix;

		CharacterData()
			:controllerId(COLLADAFW::UniqueId::INVALID)
			,rootUniqueId(COLLADAFW::UniqueId::INVALID)
			,geometryUniqueId(COLLADAFW::UniqueId::INVALID)
			,skinUniqueId(COLLADAFW::UniqueId::INVALID)
		{
		}
	};

	class AnimationData
	{
	public:
		size_t outDimension;
		Util::Array<float> times;
		Util::Array<float> values;
	};

	class MaterialData
	{
	public:
		struct TextureCoordData
		{
			COLLADAFW::TextureMapId texturemapId;
			IndexT uvSetIndex;
			Util::String Semantic;			
		};
	public:
		COLLADAFW::MaterialId materialId;
		COLLADAFW::UniqueId materialUniqueId;
		//Util::Array<TextureCoordData> textures;
		Util::Dictionary<COLLADAFW::TextureMapId,TextureCoordData> textureDatas;
	};

	class NodeData
	{
	public:
		enum NodeType
		{
			NODE,
			JOINT,
		};
	public:
		Util::String nodeName;
		COLLADAFW::UniqueId nodeUniqueId;
		NodeType nodeType;
		Math::matrix44 bind_matrix;///fixed

		COLLADAFW::UniqueId parentUniqueId;
		Util::Array<COLLADAFW::UniqueId> childUniqueIds;
		Util::Array<COLLADAFW::UniqueId> geometryUniqueIds;
		Util::Array<COLLADAFW::UniqueId> controllerUniqueIds;
		Util::Array<COLLADAFW::UniqueId> animationListUniqueIds;
		Util::Dictionary<COLLADAFW::MaterialId,MaterialData> materialDatas;

		NodeData()
			:nodeName("NODENAME")
			,nodeType(NODE)
			,bind_matrix(Math::matrix44::identity())
			,parentUniqueId(COLLADAFW::UniqueId::INVALID)
		{
		}
	};

	class EffectData
	{
	public:
		COLLADAFW::TextureMapId diffMapId;
		COLLADAFW::TextureMapId emsvMapId;
		COLLADAFW::TextureMapId specMapId;
		COLLADAFW::TextureMapId bumpMapId;

		COLLADAFW::SamplerID   diffSamplerId;
		COLLADAFW::SamplerID   emsvSamplerId;
		COLLADAFW::SamplerID   specSamplerId;
		COLLADAFW::SamplerID   bumpSamplerId;

		COLLADAFW::UniqueId diffImageUniqueId;
		COLLADAFW::UniqueId emsvImageUniqueId;
		COLLADAFW::UniqueId specImageUniqueId;
		COLLADAFW::UniqueId bumpImageUniqueId;

		bool textureFixed;

		EffectData()
			:diffMapId(-1),emsvMapId(-1),specMapId(-1),bumpMapId(-1)
			,diffSamplerId(-1),emsvSamplerId(-1),specSamplerId(-1),bumpSamplerId(-1)
			,diffImageUniqueId(COLLADAFW::UniqueId::INVALID)
			,emsvImageUniqueId(COLLADAFW::UniqueId::INVALID)
			,specImageUniqueId(COLLADAFW::UniqueId::INVALID)
			,bumpImageUniqueId(COLLADAFW::UniqueId::INVALID)
			,textureFixed(false)
		{
		}
	};

	class Nebula3Writer : public COLLADAFW::IWriter
	{
	public:

	public:
		Nebula3Writer( const COLLADABU::URI& inputFile, const COLLADABU::URI& outputFile );
		virtual ~Nebula3Writer();

	public:

		bool write();

		/** Deletes the entire scene.
		@param errorMessage A message containing informations about the error that occurred.
		*/
		void cancel(const COLLADAFW::String& errorMessage);;

		/** Prepare to receive data.*/
		void start();;

		/** Remove all objects that don't have an object. Deletes unused visual scenes.*/
		void finish();;

		/** When this method is called, the writer must write the global document asset.
		@return The writer should return true, if writing succeeded, false otherwise.*/
		virtual bool writeGlobalAsset ( const COLLADAFW::FileInfo* asset );

		/** Writes the entire visual scene.
		@return True on succeeded, false otherwise.*/
		virtual bool writeVisualScene ( const COLLADAFW::VisualScene* visualScene );

		/** Writes the scene.
		@return True on succeeded, false otherwise.*/
		virtual bool writeScene ( const COLLADAFW::Scene* scene );

		/** Handles all nodes in the library nodes.
		@return True on succeeded, false otherwise.*/
		virtual bool writeLibraryNodes( const COLLADAFW::LibraryNodes* libraryNodes );

		/** Writes the geometry.
		@return True on succeeded, false otherwise.*/
		virtual bool writeGeometry ( const COLLADAFW::Geometry* geometry );

		/** Writes the material.
		@return True on succeeded, false otherwise.*/
		virtual bool writeMaterial( const COLLADAFW::Material* material );

		/** Writes the effect.
		@return True on succeeded, false otherwise.*/
		virtual bool writeEffect( const COLLADAFW::Effect* effect );

		/** Writes the camera.
		@return True on succeeded, false otherwise.*/
		virtual bool writeCamera( const COLLADAFW::Camera* camera );

		/** Writes the image.
		@return True on succeeded, false otherwise.*/
		virtual bool writeImage( const COLLADAFW::Image* image );

		/** Writes the light.
		@return True on succeeded, false otherwise.*/
		virtual bool writeLight( const COLLADAFW::Light* light );

		/** Writes the animation.
		@return True on succeeded, false otherwise.*/
		virtual bool writeAnimation( const COLLADAFW::Animation* animation );

		/** Writes the animation.
		@return True on succeeded, false otherwise.*/
		virtual bool writeAnimationList( const COLLADAFW::AnimationList* animationList );

		/** Writes the skin controller data.
		@return True on succeeded, false otherwise.*/
		virtual bool writeSkinControllerData( const COLLADAFW::SkinControllerData* skinControllerData );

		/** Writes the controller.
		@return True on succeeded, false otherwise.*/
		virtual bool writeController( const COLLADAFW::Controller* Controller );

		/** When this method is called, the writer must write the formulas. All the formulas of the entire
		COLLADA file are contained in @a formulas.
		@return The writer should return true, if writing succeeded, false otherwise.*/
		virtual bool writeFormulas( const COLLADAFW::Formulas* formulas ){return true;}

		/** When this method is called, the writer must write the kinematics scene. 
		@return The writer should return true, if writing succeeded, false otherwise.*/
		virtual bool writeKinematicsScene( const COLLADAFW::KinematicsScene* kinematicsScene ){return true;};


	private:
        /** Disable default copy ctor. */
		Nebula3Writer( const Nebula3Writer& pre );
        /** Disable default assignment operator. */
		const Nebula3Writer& operator= ( const Nebula3Writer& pre );
		/// joint map
		typedef Util::Dictionary<COLLADAFW::UniqueId,Characters::CharacterJoint> JointMaps;
		
		void makeDirectXFriendly();
		Math::matrix44 toMatrix44(const COLLADABU::Math::Matrix4& matrix4);		
		Math::matrix44 toDirectXMatrix(const Math::matrix44& matrix44);

		void getMeshPosition(COLLADAFW::Mesh* mesh,GeometryData& geometryData);
		void getMeshNormal(COLLADAFW::Mesh* mesh,GeometryData& geometryData);
		void getMeshUVCoords(COLLADAFW::Mesh* mesh,GeometryData& geometryData);
		void getMeshFace(COLLADAFW::Mesh* mesh,GeometryData& geometryData);
		void importMesh( const COLLADAFW::Geometry* geometry );

		void importVisualScene ( const COLLADAFW::VisualScene* visualScene );
		bool importNode ( const COLLADAFW::Node* node,const COLLADAFW::Node* parentNode );
		void importTransformations (const COLLADAFW::Node* node,NodeData& nodeData);

		void importSkinController(const COLLADAFW::Controller* Controller);
		void importSkinControllerData( const COLLADAFW::SkinControllerData* skinControllerData );
		void buildSkeleton(CharacterData& characterData,const SkinData& skinData );
		void buildSkinWeights(const CharacterData& characterData,const SkinData& skinData,GeometryData& geometryData );
		void buildJoint(const NodeData& nodeData,CharacterData& characterData);

		void importAnimationCurve ( COLLADAFW::AnimationCurve* animationCurve ,AnimationData& values);
		void importAnimationList( const COLLADAFW::AnimationList* animationList );

		Math::matrix44 getGeometryMatrix(const COLLADAFW::UniqueId& nodeId);
		Ptr<TransformNode> writeGeometryData( const NodeData& instanceNodeData,GeometryData& geometryData );
		Ptr<TransformNode> writeSkinGeometryData( const NodeData& instanceNodeData,const SkinData& skinData,GeometryData& geometryData );
		Ptr<Characters::CharacterNode> writeCharacterData( const NodeData& instanceNodeData,CharacterData& characterData );
		Util::String writeAnimationData( const CharacterData& characterData ,const JointMaps& jointMaps);
		bool writeMaterialData( const NodeData& instanceNodeData,GeometryData& geometryData,IndexT groupIndex,const Ptr<ModelNode>& modelNode);
		bool writeTextureData(const COLLADAFW::UniqueId& imageId,Util::String& newTextureName);
		void writeModel(Util::String& outfilename);

		bool isCoords();///default Y
		Util::String generaterNodeName(const COLLADAFW::Node* node);
		void getMaterialBinding(NodeData& nodeData,const COLLADAFW::MaterialBindingArray& materialArray );

	private:
		COLLADABU::URI mInputFile;
		COLLADABU::URI mOutputFile;		

		Util::Dictionary<COLLADAFW::UniqueId,NodeData>			mNodeMaps;
		Util::Dictionary<COLLADAFW::UniqueId,SkinData>			mSkinDatas;
		Util::Dictionary<COLLADAFW::UniqueId,GeometryData>		mGeometryDatas;
		Util::Dictionary<COLLADAFW::UniqueId,CharacterData>		mCharacterDatas;
		Util::Dictionary<COLLADAFW::UniqueId,AnimationData>		mAnimationDatas;
		Util::Dictionary<COLLADAFW::UniqueId,Util::Array<Math::matrix44>> mAnimationListDatas;
		/// controller ID实例对应的node ID
		Util::Dictionary<COLLADAFW::UniqueId,COLLADAFW::UniqueId> mControllerNodeMaps;
		/// geometry I实例对应的node ID
		Util::Dictionary<COLLADAFW::UniqueId,COLLADAFW::UniqueId> mGeometryNodeMaps;

		Util::Dictionary<COLLADAFW::UniqueId,COLLADAFW::UniqueId> mInstanceNodeMaps;

		COLLADAFW::FileInfo::UpAxisType mUpAxisType;
		COLLADAFW::FileInfo::Unit::LinearUnit mUnit;

		Util::Dictionary<COLLADAFW::UniqueId,EffectData>			mEffectDatas;
		//Util::Dictionary<COLLADAFW::UniqueId,COLLADAFW::UniqueId>	mMaterialDatas;
		Util::Dictionary<COLLADAFW::UniqueId,Util::String>          mImageDatas;
		Util::Dictionary<COLLADAFW::UniqueId,COLLADAFW::UniqueId>	mMaterialDatas;
		Util::Dictionary<COLLADAFW::UniqueId,Util::Array<COLLADAFW::UniqueId>> mGeometryBindMaterials;
	};
} // namespace DAE2Ogre

#endif // __DAE2OGRE_OGREWRITER_H__
