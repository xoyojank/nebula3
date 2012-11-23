#pragma once
//------------------------------------------------------------------------------
/**
@class Models::ModelNode

Represents a transformation hierarchy element inside a Model. Subclasses
of ModelNodes represent transformations and geometry of a 3D model
arranged in 3d hierarchy (but not in a logical hierarchy of C++ object,
instead model nodes live in a flat array to prevent recursive iteration).

A ModelNode is roughly comparable to a nSceneNode in Nebula2.

(C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "util/stringatom.h"
#include "math/bbox.h"
#include "toolkitutil/modelwriter.h"
#include <maya/MDagPath.h>

//------------------------------------------------------------------------------
namespace Models
{
	class Model;

	class ModelNode : public Core::RefCounted
	{
		__DeclareClass(ModelNode);
	public:
		/// constructor
		ModelNode();
		/// destructor
		virtual ~ModelNode();

		// write data to stream
		virtual bool WriteDataTag(Ptr<ToolkitUtil::ModelWriter>& writer);
		/// extract a single node and recurse
		virtual MStatus ExtractNode(const MDagPath& nodePath);

		/// return true if currently attached to a Model
		bool IsAttachedToModel() const;
		/// get model this node is attached to
		const Ptr<Model>& GetModel() const;

		/// set bounding box
		void SetBoundingBox(const Math::bbox& b);
		/// get bounding box of model node
		const Math::bbox& GetBoundingBox() const;
		/// set model node name
		void SetName(const Util::StringAtom& n);
		/// get model node name
		const Util::StringAtom& GetName() const;
		/// set ModelNodeType
		void SetType(Util::String t);
		/// get the ModelNodeType
		Util::String GetType() const;

		/// set parent node
		void SetParent(const Ptr<ModelNode>& p);
		/// get parent node
		const Ptr<ModelNode>& GetParent() const;
		/// return true if node has a parent
		bool HasParent() const;
		/// add a child node
		void AddChild(const Ptr<ModelNode>& c);
		/// get child nodes
		const Util::Array<Ptr<ModelNode> >& GetChildren() const;
		/// return true if a direct child exists by name
		bool HasChild(const Util::StringAtom& name) const;
		/// get pointer to direct child by name
		const Ptr<ModelNode>& LookupChild(const Util::StringAtom& name) const;

		/// has string attr 
		bool HasStringAttr(const Util::StringAtom& attrId) const;
		/// get string attr    
		const Util::StringAtom& GetStringAttr(const Util::StringAtom& attrId) const;
		/// add string attribute
		void SetStringAttr(const Util::StringAtom& attrId, const Util::StringAtom& value);

	protected:
		Ptr<Model> model;
		Util::StringAtom name;
		Ptr<ModelNode> parent;
		Util::Array<Ptr<ModelNode> > children;
		Util::Dictionary<Util::StringAtom, IndexT> childIndexMap;
		Util::Dictionary<Util::StringAtom, Util::StringAtom> stringAttrs;
		//ModelNodeType::Code type;
		Util::String type;
		Math::bbox boundingBox;
	};

	//------------------------------------------------------------------------------
	/**
	*/
	inline void
	ModelNode::SetType(Util::String t)
	{
		this->type = t;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline Util::String
	ModelNode::GetType() const
	{
		return this->type;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline void
	ModelNode::SetName(const Util::StringAtom& n)
	{
		this->name = n;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline const Util::StringAtom&
	ModelNode::GetName() const
	{
		return this->name;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline void
	ModelNode::SetParent(const Ptr<ModelNode>& p)
	{
		this->parent = p;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline bool
	ModelNode::HasParent() const
	{
		return this->parent.isvalid();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline const Ptr<ModelNode>&
	ModelNode::GetParent() const
	{
		return this->parent;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline const Util::Array<Ptr<ModelNode> >&
	ModelNode::GetChildren() const
	{
		return this->children;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline void
	ModelNode::SetBoundingBox(const Math::bbox& b)
	{
		this->boundingBox = b;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline const Math::bbox&
	ModelNode::GetBoundingBox() const
	{
		return this->boundingBox;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline bool
	ModelNode::HasStringAttr(const Util::StringAtom& attrId) const
	{
		return this->stringAttrs.Contains(attrId);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline const Util::StringAtom& 
	ModelNode::GetStringAttr(const Util::StringAtom& attrId) const
	{
		return this->stringAttrs[attrId];
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline void
	ModelNode::SetStringAttr(const Util::StringAtom& attrId, const Util::StringAtom& value)
	{
		if (this->HasStringAttr(attrId))
		{
			this->stringAttrs[attrId] = value;
		}
		else
		{
			this->stringAttrs.Add(attrId, value);
		}
	}

} // namespace Models
//------------------------------------------------------------------------------

