//------------------------------------------------------------------------------
//  mayascenextractor.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "mayaextractors/meshextractor.h"
#include <maya/MItDag.h>
#include <maya/MItSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MGlobal.h>
#include <maya/MFnMesh.h>
#include <maya/MFnSet.h>
#include <maya/MItMeshVertex.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshFaceVertex.h>
#include <maya/MDistance.h>
#include "mayamain/mayalogger.h"
#include <mayacore/shapenode.h>
#include "mayamain/mayalogger.h"


namespace Models
{
	__ImplementClass(Models::MeshExtractor, 'MEXR', Models::ModelExtractorBase);

	using namespace Maya;
	using namespace ToolkitUtil;

	//------------------------------------------------------------------------------
	/**
	*/
	MStatus 
	MeshExtractor::ExtractNode(const Ptr<Model>& model, const MDagPath& meshDag)
	{
		MStatus stat;
		MObject component;
		//int polygonCount,vertCount;
		// Check that given DagPath corresponds to a mesh node
		if (!meshDag.hasFn(MFn::kMesh))
			return MS::kFailure;

		MFnMesh fnMesh( meshDag, &stat );
		if ( MS::kSuccess != stat) {
			return MS::kFailure;
		}

		meshName = fnMesh.name().asChar();
		this->ExtractVertices(meshDag);		
		this->ExtractPolygon(meshDag);

		// shape node 
		//Ptr<ShapeNode> shapeNode = ShapeNode::Create();
		//shapeNode->SetName(meshName);
		//shapeNode->SetMeshResourceId("meshes:"+meshName + ".nvx2");
		//shapeNode->SetPrimitiveGroupIndex(0);
		//model->AttachNode(shapeNode.upcast<ModelNode>());

		return MS::kSuccess;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	MStatus  
	MeshExtractor::ExtractVertices(const MDagPath& meshDag)
	{
		MStatus stat;
		MObject component;
		int vertIndex,vertCount;
		MVector vNormal;
		MColor color;
		float2 vUV;
		MFnMesh meshFn(meshDag);

		// vertex
		MItMeshVertex meshIter(meshDag,component,&stat);
		if( stat == MS::kSuccess)
		{			
			vertCount = meshIter.count();

			MStringArray colorSetNames;
			meshFn.getColorSetNames(colorSetNames);
			MString cColorSetName;
			meshFn.getCurrentColorSetName(cColorSetName);

			MStringArray uvSetNames;
			meshFn.getUVSetNames(uvSetNames);
			MString cuvSetName;
			meshFn.getCurrentUVSetName(cuvSetName);

			//MayaLogger::Instance()->Print("Extract Vertex Count:[%d]\n",vertCount);
			for ( ; !meshIter.isDone(); meshIter.next())
			{
				MPoint pt = meshIter.position( MSpace::kWorld );
				vertIndex = meshIter.index();
				//MayaLogger::Instance()->Print("[%d : %f %f %f]\n",vertIndex,pt.x,pt.y,pt.z);
				
				// vertex
				MeshBuilderVertex vertex;
				Math::float4 ver((float)pt.x,(float)pt.y,(float)pt.z,0);
				vertex.SetComponent(MeshBuilderVertex::CoordIndex,ver);

				// normal
				if( meshIter.getNormal(vNormal,MSpace::kWorld) == MS::kSuccess)
				{
					Math::float4 verNormal((float)vNormal.x,(float)vNormal.y,(float)vNormal.z,0);
					vertex.SetComponent(MeshBuilderVertex::NormalIndex,verNormal);
				}

				// color
				if( meshIter.getColor(color,&cColorSetName) == MS::kSuccess)
				{
					Math::float4 verColor(color.r,color.g,color.b,0);
					vertex.SetComponent(MeshBuilderVertex::ColorIndex,verColor);
				}

				// uv texture
				if( meshIter.getUV(vUV,&cuvSetName) == MS::kSuccess)
				{
					Math::float4 verTexture(vUV[0]*8192,vUV[1]*8192,0.0f,0.0f);
					vertex.SetComponent(MeshBuilderVertex::Uv0Index,verTexture);
				}

				this->meshBuilder.AddVertex(vertex);
			}

			MItMeshFaceVertex faceIter(meshDag,component,&stat);
			if( stat == MS::kSuccess)
			{
				MVector vBinormal,vTangent;
				for (;!faceIter.isDone();faceIter.next())
				{
					vertIndex = faceIter.vertId();
					MeshBuilderVertex& vertex = this->meshBuilder.VertexAt(vertIndex);
					vBinormal = faceIter.getBinormal(MSpace::kWorld,&cuvSetName,&stat);
					if( stat == MS::kSuccess)
					{
						Math::float4 verBinormal((float)vBinormal.x,(float)vBinormal.y,(float)vBinormal.z,0);
						vertex.SetComponent(MeshBuilderVertex::BinormalIndex,verBinormal);
						//MayaLogger::Instance()->Print("[%d Binormal:%f %f %f]\n",vertIndex,vBinormal.x,vBinormal.y,vBinormal.z);
					}
					vTangent  = faceIter.getTangent(MSpace::kWorld,&cuvSetName,&stat);
					if( stat == MS::kSuccess)
					{
						Math::float4 verTangent((float)vTangent.x,(float)vTangent.y,(float)vTangent.z,0);
						vertex.SetComponent(MeshBuilderVertex::TangentIndex,verTangent);
						//MayaLogger::Instance()->Print("[%d Binormal:%f %f %f]\n",vertIndex,vBinormal.x,vBinormal.y,vBinormal.z);
					}
				}
			}
		}
		return stat;
	}	

	//------------------------------------------------------------------------------
	/**
	*/
	MStatus  
	MeshExtractor::ExtractPolygon(const MDagPath& meshDag)
	{
		MStatus stat;
		MObject component;
		int polygonIndex,polygonCount,vertCount,vertIndex;

		MItMeshPolygon ployIter(meshDag,component,&stat);
		if( stat == MS::kSuccess)
		{
			polygonCount = ployIter.count();
			//MayaLogger::Instance()->Print("Extract Polygon Count:[%d]\n",polygonCount);
			for (; !ployIter.isDone(); ployIter.next())
			{
				polygonIndex = ployIter.index();
				vertCount = ployIter.polygonVertexCount();

				MeshBuilderTriangle triangle;
				for (IndexT index = 0; index < vertCount; index++)
				{
					vertIndex = ployIter.vertexIndex(index);
					//MayaLogger::Instance()->Print(" %d ",vertIndex);
				}
				triangle.SetVertexIndices(ployIter.vertexIndex(0),ployIter.vertexIndex(1),ployIter.vertexIndex(2));
				this->meshBuilder.AddTriangle(triangle);
				//MayaLogger::Instance()->Print("\n");
				
			}
		}
		return stat;
	}
	
} // namespace Maya