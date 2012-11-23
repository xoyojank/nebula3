#pragma once
//------------------------------------------------------------------------------
/**
@class Models::Model

A Model represents the template for a renderable object, consisting
of a hierarchy of ModelNodes which represent transformations and shapes.
Models should generally be created through the ModelServer, which 
guarantees that a given Model is only loaded once into memory. To
render a Model, at least one ModelInstance must be created from the
Model. Usually one ModelInstance is created per game object. Generally
speaking, all per-instance data lives in the ModelInstance objects, while
all constant data lives in the Model object. 

(C) 2007 Radon Labs GmbH
*/
#include "math/bbox.h"
#include "mayacore/modelnode.h"

//------------------------------------------------------------------------------
namespace Models
{
	class Model : public Core::RefCounted
	{
		__DeclareClass(Model);
	public:
		/// constructor
		Model();
		/// destructor
		virtual ~Model();

		/// update model's bounding box from model nodes
		void UpdateBoundingBox();
		/// set the model's local bounding box
		void SetBoundingBox(const Math::bbox& b);
		/// get the model's local bounding box
		const Math::bbox& GetBoundingBox() const;

		/// lookup a ModelNode in the Model by path, returns invalid pointer if not found
		Ptr<ModelNode> LookupNode(const Util::String& path) const;
		/// attach a model node to the Model
		void AttachNode(const Ptr<ModelNode>& node);
		/// remove a model node from the Model
		void RemoveNode(const Ptr<ModelNode>& node);
		/// access to model nodes
		const Util::Array<Ptr<ModelNode> >& GetNodes() const;
		/// get root node (always at index 0)
		const Ptr<ModelNode>& GetRootNode() const;
		/// get number of nodes in the model
		SizeT GetNumNodes() const;

		Math::bbox boundingBox;
		Util::Array<Ptr<ModelNode> > nodes;
	};

	//------------------------------------------------------------------------------
	/**
	*/
	inline void
	Model::SetBoundingBox(const Math::bbox& b)
	{
		this->boundingBox = b;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline const Math::bbox&
	Model::GetBoundingBox() const
	{
		return this->boundingBox;
	}

	//------------------------------------------------------------------------------
	/**
	Get a pointer to the root node. This is always
	the first node.
	*/
	inline const Ptr<ModelNode>&
	Model::GetRootNode() const
	{
		n_assert(!this->nodes.IsEmpty());
		n_assert(!this->nodes[0]->HasParent());
		return this->nodes[0];
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline const Util::Array<Ptr<ModelNode> >&
	Model::GetNodes() const
	{
		return this->nodes;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline SizeT
	Model::GetNumNodes() const
	{
		return this->nodes.Size();
	}
} // namespace Models
//------------------------------------------------------------------------------

