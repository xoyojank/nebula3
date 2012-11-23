//------------------------------------------------------------------------------
//  model.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "mayacore/model.h"

namespace Models
{
	__ImplementClass(Models::Model, 'MODL', Core::RefCounted);

	using namespace Util;

	//------------------------------------------------------------------------------
	/**
	*/
	Model::Model()
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	Model::~Model()
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
	Model::AttachNode(const Ptr<ModelNode>& node)
	{
		n_assert(node->GetName().IsValid());
		this->nodes.Append(node);
		//node->OnAttachToModel(this);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
	Model::RemoveNode(const Ptr<ModelNode>& node)
	{
		IndexT nodeIndex = this->nodes.FindIndex(node);
		n_assert(InvalidIndex != nodeIndex);
		//node->OnRemoveFromModel();
		this->nodes.EraseIndex(nodeIndex);
	}

	//------------------------------------------------------------------------------
	/**
	This method will update the Model's bounding box to include the 
	bounding boxes of all ModelNodes owned by the Model object.
	*/
	void
	Model::UpdateBoundingBox()
	{
		n_assert(this->nodes.Size() > 0);
		this->boundingBox.begin_extend();
		IndexT i;
		for (i = 0; i < this->nodes.Size(); i++)
		{
			this->boundingBox.extend(this->nodes[i]->GetBoundingBox());
		}
	}

	//------------------------------------------------------------------------------
	/**
	Careful, this method is SLOW!
	*/
	Ptr<ModelNode>
	Model::LookupNode(const String& path) const
	{
		Ptr<ModelNode> curPtr;
		if (this->nodes.Size() > 0)
		{
			Array<String> tokens = path.Tokenize("/");
			n_assert(tokens.Size() > 0);
			curPtr = this->GetRootNode();
			if (tokens[0] == curPtr->GetName().Value())
			{
				IndexT i;
				for (i = 1; i < tokens.Size(); i++)
				{
					StringAtom curToken = tokens[i];
					if (curPtr->HasChild(curToken))
					{
						curPtr = curPtr->LookupChild(curToken);
					}
					else
					{
						// return an invalid ptr
						return Ptr<ModelNode>();
					}
				}
			}
		}
		return curPtr;
	}

} // namespace Models
