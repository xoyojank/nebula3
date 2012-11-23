//------------------------------------------------------------------------------
//  DAE2Nebula3Writer_Geometry.cpp
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
	void Nebula3Writer::getMeshPosition(COLLADAFW::Mesh* mesh,GeometryData& geometryData)
	{
		// We have always a stride of three (x, y and z values)
		const size_t stride = 3;
		float x,y,z;
		const COLLADAFW::MeshVertexData& positions = mesh->getPositions ();
		const COLLADAFW::MeshVertexData::DataType type = positions.getType ();
		Util::Array<Math::vector>& mPositions = geometryData.mPositions;
		for ( size_t i=0; i<positions.getValuesCount(); i+=stride )
		{
			switch ( type )
			{
			case COLLADAFW::MeshVertexData::DATA_TYPE_FLOAT:
				{
					const COLLADAFW::ArrayPrimitiveType<float>* values = positions.getFloatValues ();
					x = (*values)[i]; y =(*values)[i+1]; z = (*values)[i+2];
				}
				break;
			case COLLADAFW::MeshVertexData::DATA_TYPE_DOUBLE:
				{
					const COLLADAFW::ArrayPrimitiveType<double>* values = positions.getDoubleValues ();
					x = (float)(*values)[i]; y =(float)(*values)[i+1]; z = (float)(*values)[i+2];
				}
				break;
			default:
				n_error("No valid data type for positions: %d",type );
				break;
			}
			mPositions.Append(Math::vector(x,y,z));
		}/// end for
	}

	//--------------------------------------------------------------------
	void Nebula3Writer::getMeshNormal(COLLADAFW::Mesh* mesh,GeometryData& geometryData)
	{
		if( !mesh->hasNormals()) return;
		const size_t stride = 3;
		float x,y,z;
		const COLLADAFW::MeshVertexData& normals = mesh->getNormals();
		const COLLADAFW::MeshVertexData::DataType type = normals.getType ();
		Util::Array<Math::vector>& mNormals = geometryData.mNormals;
		for ( size_t i=0; i<normals.getValuesCount(); i+=stride )
		{
			switch ( type )
			{
			case COLLADAFW::MeshVertexData::DATA_TYPE_FLOAT:
				{
					const COLLADAFW::ArrayPrimitiveType<float>* values = normals.getFloatValues ();
					x = (*values)[i]; y =(*values)[i+1]; z = (*values)[i+2];
				}
				break;
			case COLLADAFW::MeshVertexData::DATA_TYPE_DOUBLE:
				{
					const COLLADAFW::ArrayPrimitiveType<double>* values = normals.getDoubleValues ();
					x = (float)(*values)[i]; y =(float)(*values)[i+1]; z = (float)(*values)[i+2];
				}
				break;
			default:
				n_error("No valid data type for positions: %d",type );
				break;
			}
			Math::vector normal = Math::vector::normalize(Math::vector(x,y,z));
			normal.set_x( (normal.x() + 1) / 2.0f);
			normal.set_y( (normal.y() + 1) / 2.0f);
			normal.set_z( (normal.z() + 1) / 2.0f);
			mNormals.Append(normal);
		}/// end for
	}

	//--------------------------------------------------------------------
	void Nebula3Writer::getMeshUVCoords(COLLADAFW::Mesh* mesh,GeometryData& geometryData)
	{	
		const COLLADAFW::MeshVertexData& uvCoords = mesh->getUVCoords();
		if( uvCoords.getNumInputInfos() == 0) return;
		float x,y;
		size_t dataBase =0;
		const COLLADAFW::MeshVertexData::DataType type = uvCoords.getType ();
		const MeshVertexData::InputInfosArray& inputInfos = uvCoords.getInputInfosArray();
		for (unsigned int i=0; i< inputInfos.getCount();i++)
		{
			MeshVertexData::InputInfos* inputinfo = inputInfos[i];
			if( inputinfo == NULL) continue;
			Util::String uvSetName(inputinfo->mName.c_str());
			n_assert(!uvSetName.IsEmpty());

			Util::Array<Math::vector> uv_array;
			for ( size_t j=0; j< inputinfo->mLength; j+=inputinfo->mStride )
			{				
				switch ( type )
				{
				case COLLADAFW::MeshVertexData::DATA_TYPE_FLOAT:
					{
						const COLLADAFW::ArrayPrimitiveType<float>* values = uvCoords.getFloatValues ();
						x = (*values)[dataBase+j];y = (*values)[dataBase+j+1];
					}
					break;
				case COLLADAFW::MeshVertexData::DATA_TYPE_DOUBLE:
					{
						const COLLADAFW::ArrayPrimitiveType<double>* values = uvCoords.getDoubleValues ();
						x = (float)(*values)[dataBase+j];y = (float)(*values)[dataBase+j+1];
					}
					break;
				default:
					n_error("No valid data type for positions: %d",type );
					break;
				}
				Math::vector uv(x * 8192.0f,(1-y) * 8192.0f,0.0f);
				uv_array.Append(uv);
			}/// end for
			dataBase += inputinfo->mLength;
			geometryData.mUvs.Add(uvSetName,uv_array);
		}///end for
	}

	//--------------------------------------------------------------------
	void Nebula3Writer::getMeshFace(COLLADAFW::Mesh* mesh,GeometryData& geometryData)
	{
		const MeshPrimitiveArray& primitiveArray = mesh->getMeshPrimitives();
		for (size_t index =0;index< primitiveArray.getCount();index++)
		{
			MeshPrimitive* meshPrimitive = primitiveArray[index];
			n_assert( meshPrimitive->getPrimitiveType() == MeshPrimitive::TRIANGLES);
			GeometryData::MeshData meshData;
			const UIntValuesArray& positions = meshPrimitive->getPositionIndices();
			for (unsigned int j=0;j<positions.getCount();j++) meshData.positionIndices.Append(positions[j]);

			const UIntValuesArray& normals = meshPrimitive->getNormalIndices();
			for (unsigned int j=0;j<normals.getCount();j++) meshData.normalIndices.Append(normals[j]);

			if( meshPrimitive->hasUVCoordIndices())
			{
				const IndexListArray& indexList = meshPrimitive->getUVCoordIndicesArray(); 
				size_t uvCount = indexList.getCount();
				for (unsigned int j=0;j<uvCount;j++)
				{
					const IndexList* uvlist = indexList[j];
					if(uvlist ==NULL ) continue;
					IndexT uvSetIndex = uvlist->getSetIndex();
					meshData.uvDatas.Add(uvSetIndex,GeometryData::TextureData());
					GeometryData::TextureData& textureData = meshData.uvDatas[ uvSetIndex ];
					textureData.uvSetname = uvlist->getName().c_str();
					textureData.uvSetIndex = uvSetIndex;
					textureData.baseIndex = uvlist->getInitialIndex();
					textureData.stride = uvlist->getStride();
					const UIntValuesArray& uvs = uvlist->getIndices();
					for (unsigned int k=0;k<uvs.getCount();k++) 
						textureData.uvIndices.Append(uvs[k]);
				}///end for
			}///end if

			meshData.materialId = meshPrimitive->getMaterialId();
			meshData.materialName = meshPrimitive->getMaterial().c_str();
			geometryData.mMeshDatas.Append(meshData);
		}///end for
	}

	//--------------------------------------------------------------------
	void Nebula3Writer::importMesh(const COLLADAFW::Geometry* geometry)
	{
		const COLLADAFW::UniqueId& uniqueId = geometry->getUniqueId ();
		COLLADAFW::Mesh* mesh = ( COLLADAFW::Mesh* ) geometry;
		this->mGeometryDatas.Add(uniqueId,GeometryData());
		GeometryData& geometryData = this->mGeometryDatas[uniqueId];
		geometryData.mGeometryUniqueId = uniqueId;
		geometryData.mGeometryName = geometry->getOriginalId().c_str();
		n_assert(!geometryData.mGeometryName.IsEmpty());
		this->getMeshPosition(mesh,geometryData);
		this->getMeshNormal(mesh,geometryData);
		this->getMeshUVCoords(mesh,geometryData);
		this->getMeshFace(mesh,geometryData);
	}

	//--------------------------------------------------------------------
	bool Nebula3Writer::writeGeometry( const COLLADAFW::Geometry* geometry )
	{		
		if( geometry->getType() == Geometry::GEO_TYPE_MESH)		
		{
			this->importMesh(geometry);
		}
	
		return true;
	}

	//--------------------------------------------------------------------
	Ptr<TransformNode> Nebula3Writer::writeGeometryData( const NodeData& instanceNodeData,GeometryData& geometryData )
	{
		Ptr<TransformNode> transformNode = TransformNode::Create();
		MeshBuilder meshBuilder;
		Util::String meshFilename =MESH_PRE_FIXED;
		meshFilename += geometryData.mGeometryName;meshFilename.Append(".nvx2");	

		for (int groupIndex=0;groupIndex<geometryData.mMeshDatas.Size();groupIndex++)
		{
			GeometryData::MeshData& meshData = geometryData.mMeshDatas[groupIndex];	

			/// 设置基本信息
			Ptr<ShapeNode> shapeNode = ShapeNode::Create();
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

		/// 设置位置信息
		Math::vector outTranslate;Math::quaternion outRotate;Math::vector outScale;	
		Math::matrix44 matTemp = this->getGeometryMatrix(instanceNodeData.nodeUniqueId);
		matTemp.decompose(outScale,outRotate,outTranslate);	
		transformNode->SetPosition(outTranslate);
		transformNode->SetRotation(outRotate);
		transformNode->SetScale(outScale);	

		Math::bbox boundingbox = meshBuilder.ComputeBoundingBox();
		boundingbox.transform(matTemp);
		transformNode->SetName(geometryData.mGeometryName);
		transformNode->SetBoundingBox(boundingbox);
		Util::Array<Ptr<ModelNode>> shapeNodes = transformNode->GetChildren();
		for (int i=0;i<shapeNodes.Size();i++)
		{
			shapeNodes[i]->SetBoundingBox(boundingbox);
		}///end for


		return transformNode;
	}

}