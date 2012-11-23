//------------------------------------------------------------------------------
//  shapenode.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "mayacore/shapenode.h"
#include "mayamain/mayalogger.h"
#include <maya/MFnMesh.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MPlug.h>
#include <toolkitutil/meshutil/meshbuildersaver.h>

namespace Models
{
__ImplementClass(Models::ShapeNode, 'SPND', Models::StateNode);

using namespace Util;
using namespace IO;
using namespace Maya;
using namespace ToolkitUtil;
#define PRECISION 0.0001

//------------------------------------------------------------------------------
/**
*/
ShapeNode::ShapeNode() :
    primGroupIndex(InvalidIndex)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ShapeNode::~ShapeNode()
{    
}

//------------------------------------------------------------------------------
/**
*/
MStatus 
ShapeNode::getUVSets(const MDagPath& meshDag)
{
	MFnMesh mesh(meshDag);
	MStatus stat;
	// Get uv texture coordinate sets' names
	if (mesh.numUVSets() > 0)
	{
		stat = mesh.getUVSetNames(newuvsets);
		if (MS::kSuccess != stat)
		{
			MayaLogger::Instance()->Print("Error retrieving UV sets names\n");
			return MS::kFailure;
		}
	}
	// Save uvsets info
	for (unsigned int i = m_uvsets.size(); i < m_uvsets.size();i++)
	{
		uvset uv;
		uv.size = 2;
		m_uvsets.push_back(uv);
	}
	return MS::kSuccess;
}

//------------------------------------------------------------------------------
/**
*/
MStatus 
ShapeNode::getVertices(const MDagPath &meshDag)
{
	MFnMesh mesh(meshDag);
	// prepare vertex table
	newvertices.resize(mesh.numVertices());
	newweights.resize(mesh.numVertices());
	newjointIds.resize(mesh.numVertices());
	for(unsigned int i=0;i<newvertices.size();i++)
	{
		newvertices[i].pointIdx = -1;
		newvertices[i].normalIdx = -1;
		newvertices[i].next = -2;
	}
	//get vertex positions from mesh
	mesh.getPoints(newpoints,MSpace::kWorld);
	//get list of normals from mesh data
	mesh.getNormals(newnormals,MSpace::kWorld);
	//check the "opposite" attribute to see if we have to flip normals
	mesh.findPlug("opposite",true).getValue(opposite);
	return MS::kSuccess;
}

//------------------------------------------------------------------------------
/**
*/
MStatus 
ShapeNode::getFaces(const MDagPath &meshDag)
{
	MStatus stat;
	MFnMesh mesh(meshDag);
	// create an iterator to go through mesh polygons
	if (mesh.numPolygons() > 0)
	{
		MItMeshPolygon faceIter(mesh.object(),&stat);
		if (MS::kSuccess != stat)
		{
			MayaLogger::Instance()->Print("Error accessing mesh polygons\n");
			return MS::kFailure;
		}

		MayaLogger::Instance()->Print("num polygons =%d\n",mesh.numPolygons());
		// iterate over mesh polygons
		for (; !faceIter.isDone(); faceIter.next())
		{
			bool different;
			int numTris=0;
			faceIter.numTriangles(numTris);
			// for every triangle composing current polygon extract triangle info
			for (int iTris=0; iTris<numTris; iTris++)
			{				
				int idx;
				// create a new face to store triangle info
				face newFace;
				// extract triangle vertex indices
				MPointArray triPoints;
				MIntArray tempTriVertexIdx,triVertexIdx;
				faceIter.getTriangle(iTris,triPoints,tempTriVertexIdx);
				// convert indices to face-relative indices
				MIntArray polyIndices;
				faceIter.getVertices(polyIndices);
				for(unsigned int iObj=0; iObj < tempTriVertexIdx.length(); ++iObj)
				{
					// iPoly is face-relative vertex index
					for (unsigned int iPoly=0; iPoly < polyIndices.length(); ++iPoly)
					{
						if (tempTriVertexIdx[iObj] == polyIndices[iPoly]) 
						{
							triVertexIdx.append(iPoly);
							break;
						}
					}
				}
				// iterate over triangle's vertices
				for (int i=0; i<3; i++)
				{
					different = true;
					int vtxIdx = faceIter.vertexIndex(triVertexIdx[i],&stat);
					if (stat != MS::kSuccess)
					{
						MayaLogger::Instance()->Print("Could not access vertex position\n");
						return MS::kFailure;
					}
					int nrmIdx = faceIter.normalIndex(triVertexIdx[i],&stat);
					if (stat != MS::kSuccess)
					{
						MayaLogger::Instance()->Print("Could not access vertex normal\n");
						return MS::kFailure;
					}

					// get vertex color
					MColor color;
					if (faceIter.hasColor(triVertexIdx[i]))
					{
						stat = faceIter.getColor(color,triVertexIdx[i]);
						if (MS::kSuccess != stat)
						{
							color = MColor(1,1,1,1);
						}
						if (color.r > 1)
							color.r = 1;
						else if (color.r < PRECISION)
							color.r = 0;
						if (color.g > 1)
							color.g = 1;
						else if (color.g < PRECISION)
							color.g = 0;
						if (color.b > 1)
							color.b = 1;
						else if (color.b < PRECISION)
							color.b = 0;
						if (color.a > 1)
							color.a = 1;
						else if (color.a < PRECISION)
							color.a = 0;
					}
					else
					{
						color = MColor(1,1,1,1);
					}
					// first time we encounter a vertex in this position
					if (newvertices[vtxIdx].next == -2)	
					{
						// save vertex position
						newpoints[vtxIdx].cartesianize();
						newvertices[vtxIdx].pointIdx = vtxIdx;
						// save vertex normal
						newvertices[vtxIdx].normalIdx = nrmIdx;
						// save vertex colour
						newvertices[vtxIdx].r = color.r;
						newvertices[vtxIdx].g = color.g;
						newvertices[vtxIdx].b = color.b;
						newvertices[vtxIdx].a = color.a;
						// save vertex texture coordinates
						newvertices[vtxIdx].u.resize(newuvsets.length());
						newvertices[vtxIdx].v.resize(newuvsets.length());
						// save uv sets data
						for (unsigned int j=0; j<newuvsets.length(); j++)
						{
							float2 uv;
							stat = faceIter.getUV(triVertexIdx[i],uv,&newuvsets[j]);
							if (MS::kSuccess != stat)
							{
								uv[0] = 0;
								uv[1] = 0;
							}
							newvertices[vtxIdx].u[j] = uv[0];
							newvertices[vtxIdx].v[j] = (-1)*(uv[1]-1);
						}
						// save vertex index in face info
						newFace.v[i] = m_sharedGeom.vertices.size() + vtxIdx;
						// update value of index to next vertex info (-1 means nothing next)
						newvertices[vtxIdx].next = -1;
					}
					else	
					{
						// already found at least 1 vertex in this position
						// check if a vertex with same attributes has been saved already
						for (unsigned int k=vtxIdx; k!=-1 && different; k=newvertices[k].next)
						{
							different = false;

							MFloatVector n1 = newnormals[newvertices[k].normalIdx];
							MFloatVector n2 = newnormals[nrmIdx];
							if (n1.x!=n2.x || n1.y!=n2.y || n1.z!=n2.z)
							{
								different = true;
							}

							if ((newvertices[k].r!=color.r || newvertices[k].g!=color.g || newvertices[k].b!= color.b || newvertices[k].a!=color.a))
							{
								different = true;
							}

							for (unsigned int j=0; j<newuvsets.length(); j++)
							{
								float2 uv;
								stat = faceIter.getUV(triVertexIdx[i],uv,&newuvsets[j]);
								if (MS::kSuccess != stat)
								{
									uv[0] = 0;
									uv[1] = 0;
								}
								uv[1] = (-1)*(uv[1]-1);
								if (newvertices[k].u[j]!=uv[0] || newvertices[k].v[j]!=uv[1])
								{
									different = true;
								}
							}

							idx = k;
						}
						// if no identical vertex has been saved, then save the vertex info
						if (different)
						{
							vertexInfo vtx;
							// save vertex position
							vtx.pointIdx = vtxIdx;
							// save vertex normal
							vtx.normalIdx = nrmIdx;
							// save vertex colour
							vtx.r = color.r;
							vtx.g = color.g;
							vtx.b = color.b;
							vtx.a = color.a;
							// save vertex texture coordinates
							vtx.u.resize(newuvsets.length());
							vtx.v.resize(newuvsets.length());
							for (unsigned int j=0; j<newuvsets.length(); j++)
							{
								float2 uv;
								stat = faceIter.getUV(triVertexIdx[i],uv,&newuvsets[j]);
								if (MS::kSuccess != stat)
								{
									uv[0] = 0;
									uv[1] = 0;
								}
								if (fabs(uv[0]) < PRECISION)
									uv[0] = 0;
								if (fabs(uv[1]) < PRECISION)
									uv[1] = 0;
								vtx.u[j] = uv[0];
								vtx.v[j] = (-1)*(uv[1]-1);
							}
							vtx.next = -1;
							newvertices.push_back(vtx);
							// save vertex index in face info
							newFace.v[i] = m_sharedGeom.vertices.size() + newvertices.size()-1;
							newvertices[idx].next = newvertices.size()-1;
						}
						else
						{
							newFace.v[i] = m_sharedGeom.vertices.size() + idx;
						}
					}
				} // end iteration of triangle vertices
				// add face info to the array corresponding to the submesh it belongs
				// skip faces with no shaders assigned
				if (shaderPolygonMapping[faceIter.index()] >= 0)
					polygonSets[shaderPolygonMapping[faceIter.index()]].push_back(newFace);
			} // end iteration of triangles
		}
	}

	MayaLogger::Instance()->Print("done reading mesh triangles\n");
	return MS::kSuccess;
}

//------------------------------------------------------------------------------
/**
*/
MStatus 
ShapeNode::getShaders(const MDagPath& meshDag)
{
	MStatus stat;
	MFnMesh mesh(meshDag);
	stat = mesh.getConnectedShaders(0,shaders,shaderPolygonMapping);
	//std::cout.flush();
	if (MS::kSuccess != stat)
	{
		MayaLogger::Instance()->Print("Error getting connected shaders\n");
		return MS::kFailure;
	}

	MayaLogger::Instance()->Print("Found %d  connected shaders\n",shaders.length());
	if (shaders.length() <= 0)
	{
		MayaLogger::Instance()->Print("No connected shaders, skipping mesh\n");
		return MS::kFailure;
	}
	// create a series of arrays of faces for each different submesh
	polygonSets.clear();
	polygonSets.resize(shaders.length());
	return MS::kSuccess;
}

//------------------------------------------------------------------------------
/**
*/
MStatus 
ShapeNode::buildSharedGeometry(const MDagPath &meshDag)
{
	// load shared vertices
	for (unsigned int i=0; i<newvertices.size(); i++)
	{
		//MeshBuilder::Vertex vertex;
		MeshBuilderVertex vertex;
		vertexInfo vInfo = newvertices[i];
		// save vertex coordinates (rescale to desired length unit)
		MPoint point = newpoints[vInfo.pointIdx] ;//* params.lum;
		if (fabs(point.x) < PRECISION)
			point.x = 0;
		if (fabs(point.y) < PRECISION)
			point.y = 0;
		if (fabs(point.z) < PRECISION)
			point.z = 0;

		Math::float4 coord((float)point.x,(float)point.y,(float)point.z,0.0f);
		vertex.SetComponent(MeshBuilderVertex::CoordIndex,coord);

		// save vertex normal
		MFloatVector normal = newnormals[vInfo.normalIdx];
		if (fabs(normal.x) < PRECISION)
			normal.x = 0;
		if (fabs(normal.y) < PRECISION)
			normal.y = 0;
		if (fabs(normal.z) < PRECISION)
			normal.z = 0;
		if (opposite)
		{
			Math::float4 normalVec((float)-normal.x,(float)-normal.y,(float)-normal.z,0.0f);
			vertex.SetComponent(MeshBuilderVertex::NormalIndex,Math::float4::normalize(normalVec));
		}
		else
		{
			Math::float4 normalVec((float)(normal.x+1)/2,(float)(normal.y+1)/2,(float)(normal.z+1)/2,0.0f);
			vertex.SetComponent(MeshBuilderVertex::NormalIndex,normalVec);
		}

		// save vertex color
		Math::float4 colorVec((float)vInfo.r,(float)vInfo.g,(float)vInfo.b,0.0f);
		vertex.SetComponent(MeshBuilderVertex::ColorIndex,Math::float4::normalize(colorVec));

		// save texture coordinates
		for (unsigned int k=0; k<vInfo.u.size(); k++)
		{
			Math::float4 uvVec((float)vInfo.u[k]*8192,(float)vInfo.v[k]*8192,0.0f,0.0f);
			IndexT index = (int)MeshBuilderVertex::Uv0Index + k;
			vertex.SetComponent((MeshBuilderVertex::ComponentIndex)index ,uvVec);
		}
		// save vertex index in maya mesh, to retrieve future positions of the same vertex
		//v.index = vInfo.pointIdx;
		this->meshBuilder.AddVertex(vertex);
	}

	MayaLogger::Instance()->Print("done creating vertices list\n");
	return MS::kSuccess;
}

//------------------------------------------------------------------------------
/**
*/
bool
ShapeNode::WriteDataTag(Ptr<ToolkitUtil::ModelWriter>& writer)
{
	if(StateNode::WriteDataTag(writer))
	{
		if(MeshBuilderSaver::SaveNvx2(meshResId.AsCharPtr(),meshExtractor.GetMeshBuilder(),Platform::Win32))
		{
			MayaLogger::Instance()->Print("Saved mesh node %s!\n",meshResId.AsCharPtr());
//#ifdef _DEBUG
//			Util::String meshName = meshResId;
//			meshName.Append(".n3d2");
//			//meshBuilder.SaveN3d2(meshName.AsCharPtr());
//#endif
			writer->BeginTag("Mesh", FourCC('MESH'));
			writer->WriteString(meshResId);
			writer->EndTag();

			writer->BeginTag("PrimitiveGroupIndex", FourCC('PGRI'));
			writer->WriteInt(primGroupIndex);
			writer->EndTag();
			return true;
		}		
	}	

	return false;
}

//------------------------------------------------------------------------------
/**
*/
MStatus 
ShapeNode::ExtractNode(const MDagPath& meshDag)
{
	if(StateNode::ExtractNode(meshDag))
	{
		MStatus stat = meshExtractor.ExtractNode(this->model,meshDag);
		//MObject component;

		//MFnMesh fnMesh( meshDag, &stat );
		if ( MS::kSuccess != stat) return MS::kFailure;
		//if(fnMesh.isIntermediateObject()) return MS::kFailure;

		// MESH Name
		Util::String meshName = meshExtractor.GetMeshName();//fnMesh.name().asChar();//
		this->SetName(meshName);
		this->SetMeshResourceId("meshes:"+meshName + ".nvx2");
		this->SetPrimitiveGroupIndex(0);

		//newvertices.clear();
		//newweights.clear();
		//newjointIds.clear();
		//newuvsets.clear();
		//newpoints.clear();
		//newnormals.clear();
		//opposite = false;
		//shaders.clear();
		//shaderPolygonMapping.clear();
		//polygonSets.clear();

		//// Get UVSets
		//stat = getUVSets(meshDag);		
		//if (stat != MS::kSuccess)
		//{
		//	return MS::kFailure;
		//}

		//// Get connected shaders
		//stat = getShaders(meshDag);
		//if (stat != MS::kSuccess)
		//{
		//	MayaLogger::Instance()->Print("Error getting shaders connected to current mesh\n");
		//	return MS::kFailure;
		//}

		//// Get vertex data
		//stat = getVertices(meshDag);
		//if (stat != MS::kSuccess)
		//{
		//	MayaLogger::Instance()->Print("Error retrieving vertex data for current mesh\n");
		//	return MS::kFailure;
		//}

		//// Get faces data
		//stat = getFaces(meshDag);
		//if (stat != MS::kSuccess)
		//{
		//	MayaLogger::Instance()->Print("Error retrieving faces data for current mesh\n");
		//	return MS::kFailure;
		//}

		//stat = buildSharedGeometry(meshDag);
		//if (stat != MS::kSuccess)
		//{
		//	MayaLogger::Instance()->Print("Error building shared geometry for current mesh\n");
		//	return MS::kFailure;
		//}

		//int groupID = 0;
		//for (unsigned int i=0;i<polygonSets.size();i++)
		//{
		//	faceArray& trianglelist = polygonSets[i];
		//	for (unsigned int k=0;k< trianglelist.size();k++)
		//	{
		//		face& f= trianglelist[k];
		//		MeshBuilderTriangle triangle;
		//		triangle.SetVertexIndices(f.v[0],f.v[1],f.v[2]);				
		//		triangle.SetGroupId(groupID);
		//		this->meshBuilder.AddTriangle(triangle);
		//	}		
		//}	

		//n_assert(polygonSets.size() == 1);
		//this->SetPrimitiveGroupIndex(groupID);
		return MS::kSuccess;
	}
	return MS::kFailure;
}

} // namespace Models