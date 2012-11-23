//------------------------------------------------------------------------------
//  DAE2Nebula3Writer_VisualScene.cpp
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

	void Nebula3Writer::importVisualScene ( const COLLADAFW::VisualScene* visualScene )
	{
		// Iterate over the root nodes of the current visual scene.
		const COLLADAFW::NodePointerArray& rootNodes = visualScene->getRootNodes ();
		// Iterate over the root nodes.
		size_t count = rootNodes.getCount ();
		for ( size_t i=0; i<count; ++i )
		{
			// Get the root node.
			COLLADAFW::Node* rootNode = rootNodes [i];
			// Import the root node and all child nodes.
			importNode ( rootNode, 0);
		}
	}

	//--------------------------------------------------------------------
	Util::String Nebula3Writer::generaterNodeName(const COLLADAFW::Node* node)
	{
		static int nodeCount = 0;
		static Util::String nodeName = "DEFAULT_NODENAME_";
		Util::String nodeNameStr;
		if( !node->getName().empty() ) 
		{
			nodeNameStr = node->getName().c_str();			
		}
		else if( !node->getOriginalId().empty())
		{
			nodeNameStr = node->getOriginalId().c_str();
		}
		else
		{
			nodeNameStr = nodeName;
			nodeNameStr.AppendInt(++nodeCount);
		}
		return nodeNameStr;
	}

	//--------------------------------------------------------------------
	bool Nebula3Writer::importNode ( const COLLADAFW::Node* node,const COLLADAFW::Node* parentNode)
	{
		const COLLADAFW::UniqueId& uniqueId = node->getUniqueId ();			
		this->mNodeMaps.Add(uniqueId,NodeData());
		NodeData& nodeData = this->mNodeMaps[uniqueId];
		nodeData.nodeName = this->generaterNodeName(node);
		nodeData.nodeUniqueId = uniqueId;
		if( node->getType() == COLLADAFW::Node::JOINT ) nodeData.nodeType = NodeData::JOINT;
		this->importTransformations(node,nodeData);
		//n_printf("importNode [%s] ......\n",nodeData.nodeName.AsCharPtr());
		
		/// ¸¸×Ó¹ØÏµ
		if( parentNode!=NULL ) 
		{
			nodeData.parentUniqueId = parentNode->getUniqueId();			
		}	

		if( node->getType() == COLLADAFW::Node::JOINT )
		{
			const Util::Array<COLLADAFW::UniqueId>& keys = this->mCharacterDatas.KeysAsArray();
			for (int i=0; i< keys.Size();i++)
			{
				const COLLADAFW::UniqueId& jointId = keys[i];
				CharacterData& characterData = this->mCharacterDatas[jointId];
				if( characterData.rootUniqueId.isValid()) continue;
				characterData.rootUniqueId = uniqueId;
			}///end for
		}	

		// Go through the controller instances and save the ids to the current node.
		const COLLADAFW::InstanceControllerArray& controllerInstances = node->getInstanceControllers ();
		size_t numInstances = controllerInstances.getCount ();
		for ( size_t i=0; i<numInstances; ++i )
		{
			const COLLADAFW::InstanceController* instanceController = controllerInstances [i];
			const COLLADAFW::UniqueId& controllerId = instanceController->getInstanciatedObjectId ();
			mControllerNodeMaps.Add(controllerId,uniqueId);
			mInstanceNodeMaps.Add(controllerId,uniqueId);
			nodeData.controllerUniqueIds.Append(controllerId);

			const MaterialBindingArray& materialArray = instanceController->getMaterialBindings();
			this->getMaterialBinding(nodeData,materialArray);
		}

		// Go through the geometry instances and save the ids to the current node.
		const COLLADAFW::InstanceGeometryPointerArray& geometryInstances = node->getInstanceGeometries ();
		numInstances = geometryInstances.getCount ();
		for ( size_t i=0; i<numInstances; ++i )
		{
			const COLLADAFW::InstanceGeometry* instanceGeometry = geometryInstances [i];
			const COLLADAFW::UniqueId& geometryId = instanceGeometry->getInstanciatedObjectId ();
			mGeometryNodeMaps.Add(geometryId,uniqueId);
			mInstanceNodeMaps.Add(geometryId,uniqueId);
			nodeData.geometryUniqueIds.Append(geometryId);

			const MaterialBindingArray& materialArray = instanceGeometry->getMaterialBindings();
			this->getMaterialBinding(nodeData,materialArray);
			
		}///end for

		// Recursive call for all child elements.
		const COLLADAFW::NodeArray& childNodes = node->getChildNodes ();
		size_t numChildNodes = childNodes.getCount ();
		for ( size_t i=0; i<numChildNodes; ++i )
		{
			COLLADAFW::Node* childNode = childNodes [i];
			nodeData.childUniqueIds.Append(childNode->getUniqueId());
		}

		for ( size_t i=0; i<numChildNodes; ++i )
		{
			COLLADAFW::Node* childNode = childNodes [i];		
			importNode ( childNode, node);
		}
		return true;
	}

	//--------------------------------------------------------------------
	void Nebula3Writer::getMaterialBinding(NodeData& nodeData,const COLLADAFW::MaterialBindingArray& materialArray )
	{
		for (size_t j=0;j< materialArray.getCount();j++)
		{
			const MaterialBinding& material = materialArray[j];	
			const COLLADAFW::MaterialId materialId = material.getMaterialId();
			nodeData.materialDatas.Add(materialId,MaterialData());
			MaterialData& materialData = nodeData.materialDatas[materialId];
			materialData.materialId = materialId;
			materialData.materialUniqueId = material.getReferencedMaterial();
			const COLLADAFW::TextureCoordinateBindingArray& textureInfos = material.getTextureCoordinateBindingArray();
			for (size_t k=0;k< textureInfos.getCount();k++)
			{
				const TextureCoordinateBinding& textureInfo = textureInfos[k];
				const COLLADAFW::TextureMapId textureMapId = textureInfo.getTextureMapId();
				materialData.textureDatas.Add(textureMapId,MaterialData::TextureCoordData());
				MaterialData::TextureCoordData& textureData = materialData.textureDatas[textureMapId];
				textureData.texturemapId = textureMapId;
				textureData.uvSetIndex = textureInfo.getSetIndex();
				textureData.Semantic = textureInfo.getSemantic().c_str();
			}///end for
		}///end for
	}

	//--------------------------------------------------------------------
	void Nebula3Writer::importTransformations (const COLLADAFW::Node* node,NodeData& nodeData)
	{
		COLLADABU::Math::Matrix4 nodeMatrix = node->getTransformationMatrix();
		nodeData.bind_matrix = this->toMatrix44(nodeMatrix);
		const COLLADAFW::TransformationArray& transforms = node->getTransformations ();
		size_t numTransforms = transforms.getCount ();
		for ( size_t i=0; i<numTransforms; ++i )
		{
			const COLLADAFW::Transformation* transformation = transforms [i];
			COLLADAFW::Transformation::TransformationType transformType; 
			transformType = transformation->getTransformationType ();
			const COLLADAFW::UniqueId& animationListId = transformation->getAnimationList();
			if( animationListId.isValid())
			{
				nodeData.animationListUniqueIds.Append(animationListId);
				//n_printf("importTransformations [%s]  [%s]......\n",node->getName().c_str(),animationListId.toAscii().c_str());
			}			
		}///end for
	}

	//--------------------------------------------------------------------
	Math::matrix44 Nebula3Writer::getGeometryMatrix(const COLLADAFW::UniqueId& nodeId)
	{
		Math::matrix44 matTemp = Math::matrix44::identity();
		const Math::matrix44 scalealeMatrix = Math::matrix44::scaling(1,1,-1);
		if( !this->mNodeMaps.Contains(nodeId)) return matTemp;

		NodeData& nodeData = this->mNodeMaps[nodeId];
		matTemp = Math::matrix44::multiply(matTemp,nodeData.bind_matrix);
		COLLADAFW::UniqueId parentUniqueId = nodeData.parentUniqueId;
		while( parentUniqueId.isValid())
		{
			if( !this->mNodeMaps.Contains(parentUniqueId)) break;
			NodeData& parentNodeData = this->mNodeMaps[parentUniqueId];
			matTemp = Math::matrix44::multiply(matTemp,parentNodeData.bind_matrix);
			parentUniqueId = parentNodeData.parentUniqueId;
		}///end while

		Math::matrix44 matR = Math::matrix44::rotationx( -90.0f/180.0f * PI );
		matTemp = Math::matrix44::multiply(matTemp,matR);

		matR = Math::matrix44::rotationy( 90.0f/180.0f * PI);
		matTemp = Math::matrix44::multiply(matTemp,matR);
		return matTemp;
	}
}