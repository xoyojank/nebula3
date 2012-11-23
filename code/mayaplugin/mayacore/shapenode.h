#pragma once
//------------------------------------------------------------------------------
/**
    @class Models::ShapeNode
  
    ModelNode which describes a simple static shape. Since the ShapeNode
    is derived from the StateNode (which in turn is derived from the
    TransformNode), a ShapeNode contains all the necessary information
    for rendering a mesh.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "mayacore/statenode.h"
#include <maya/MPointArray.h>
#include <maya/MFloatVectorArray.h>
#include <mayautil/vertex.h>
#include <maya/MStringArray.h>
#include <maya/MFloatArray.h>
#include <maya/MIntArray.h>
#include <maya/MObjectArray.h>
#include <toolkitutil/meshutil/meshbuilder.h>
#include <mayaextractors/meshextractor.h>

//------------------------------------------------------------------------------
namespace Models
{
class ShapeNode : public StateNode
{
    __DeclareClass(ShapeNode);
public:
    /// constructor
    ShapeNode();
    /// destructor
    virtual ~ShapeNode();

	// write data to stream
	virtual bool WriteDataTag(Ptr<ToolkitUtil::ModelWriter>& writer);
	/// extract a single node and recurse
	virtual MStatus ExtractNode(const MDagPath& nodePath);
	
    /// set mesh resource id
    void SetMeshResourceId(const Util::String& resId);
    /// set primitive group index
    void SetPrimitiveGroupIndex(IndexT i);

private:
	//get uvsets info from the maya mesh
	MStatus getUVSets(const MDagPath& meshDag);
	//get vertex data
	MStatus getVertices(const MDagPath& meshDag);
	//get faces data
	MStatus getFaces(const MDagPath& meshDag);
	//build shared geometry
	MStatus buildSharedGeometry(const MDagPath& meshDag);
	//get connected shaders
	MStatus getShaders(const MDagPath& meshDag);

protected:
	Util::String meshResId;
    IndexT primGroupIndex;

	MeshExtractor meshExtractor;
	///
	ToolkitUtil::MeshBuilder meshBuilder;
	MStringArray newuvsets;
	std::vector<uvset> m_uvsets;
	std::vector<vertexInfo> newvertices;
	std::vector<MFloatArray> newweights;
	std::vector<MIntArray> newjointIds;
	MPointArray newpoints;
	MFloatVectorArray newnormals;
	MFloatVectorArray newtangents;
	MFloatVectorArray newbinormals;
	sharedGeometry m_sharedGeom;
	MIntArray shaderPolygonMapping;
	std::vector<faceArray> polygonSets;
	MObjectArray shaders;
	bool opposite;
};

//------------------------------------------------------------------------------
/**
*/
inline void
ShapeNode::SetMeshResourceId(const Util::String& resId)
{
    this->meshResId = resId;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShapeNode::SetPrimitiveGroupIndex(IndexT i)
{
    this->primGroupIndex = i;
}

} // namespace Models
//------------------------------------------------------------------------------
