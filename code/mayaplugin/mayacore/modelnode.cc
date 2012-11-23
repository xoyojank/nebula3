//------------------------------------------------------------------------------
//  modelnode.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "mayacore/modelnode.h"
#include "mayacore/model.h"

namespace Models
{
	__ImplementClass(Models::ModelNode, 'MDND', Core::RefCounted);

	using namespace Math;
	using namespace Util;

	//------------------------------------------------------------------------------
	/**
	*/
	ModelNode::ModelNode()
	{
		this->type = "Solid";//ModelNodeType::FromName(ModelNodeType::Name("Solid"));
	}

	//------------------------------------------------------------------------------
	/**
	*/
	ModelNode::~ModelNode()
	{
	}

	//------------------------------------------------------------------------------
	/**
	*/
	const Ptr<Model>&
	ModelNode::GetModel() const
	{
		return this->model;
	}


	//------------------------------------------------------------------------------
	/**
	*/
	void
	ModelNode::AddChild(const Ptr<ModelNode>& c)
	{
		n_assert(!this->childIndexMap.Contains(c->GetName()));
		this->children.Append(c);
		this->childIndexMap.Add(c->GetName(), this->children.Size() - 1);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	bool
	ModelNode::HasChild(const StringAtom& name) const
	{
		return this->childIndexMap.Contains(name);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	const Ptr<ModelNode>&
	ModelNode::LookupChild(const StringAtom& name) const
	{
		return this->children[this->childIndexMap[name]];
	}

	//------------------------------------------------------------------------------
	/**
	*/
	bool 
	ModelNode::WriteDataTag(Ptr<ToolkitUtil::ModelWriter>& writer)
	{
		// bounding box
		writer->BeginTag("LocalBox", FourCC('LBOX'));
		writer->WriteFloat4(boundingBox.center());
		writer->WriteFloat4(boundingBox.extents());
		writer->EndTag();

		// string attribute		
		return true;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	MStatus 
	ModelNode::ExtractNode(const MDagPath& nodePath)
	{
		return MS::kSuccess;
	}

} // namespace Models