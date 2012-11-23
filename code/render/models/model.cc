//------------------------------------------------------------------------------
//  model.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/model.h"
#include "models/modelinstance.h"
#include "models/modelserver.h"
#include "models/visresolver.h"
#include "models/nodes/transformnodeinstance.h"

namespace Models
{
__ImplementClass(Models::Model, 'MODL', Resources::Resource);

using namespace Util;
using namespace Resources;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
Model::Model() :
    inLoadResources(false),
    resourcesLoaded(false)
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
Model::Unload()
{    
    // discard model nodes
    IndexT i;
    for (i = 0; i < this->nodes.Size(); i++)
    {
        this->nodes[i]->OnRemoveFromModel();
    }
    this->nodes.Clear();
    this->visibleModelNodes.Reset();

    // discard instances
    for (i = 0; i < this->instances.Size(); i++)
    {
        this->instances[i]->Discard();
    }
    this->instances.Clear();

    // call parent class
    Resource::Unload();
}

//------------------------------------------------------------------------------
/**
    This method asks all model nodes to load their resources. Note that 
    actual resource loading may be asynchronous and placeholder resources
    may be in place right after this method returns.
*/
void
Model::LoadResources()
{
    n_assert(!this->inLoadResources);
    this->inLoadResources = true;
    IndexT i;
    for (i = 0; i < this->nodes.Size(); i++)
    {
        this->nodes[i]->LoadResources();
    }
}

//------------------------------------------------------------------------------
/**
    This method asks all model nodes to unload their resources.
*/
void
Model::UnloadResources()
{
    n_assert(this->inLoadResources);
    IndexT i;
    for (i = 0; i < this->nodes.Size(); i++)
    {
        this->nodes[i]->UnloadResources();
    }
    this->inLoadResources = false;
}

//------------------------------------------------------------------------------
/**
    This checks whether all resource have been loaded, if yes the method
    OnResourcesLoaded() will be called once. If some resources are not
    loaded yet, the method will return false.
*/
bool
Model::CheckPendingResources()
{
    n_assert(this->inLoadResources);
    Resource::State result = Resource::Initial;
    IndexT i;
    for (i = 0; i < this->nodes.Size(); i++)
    {
        Resource::State state = this->nodes[i]->GetResourceState();
        if (state > result)
        {
            result = state;
        }
    }
    if (Resource::Loaded == result)
    {
        if (!this->resourcesLoaded)
        {
            // all resources have been loaded
            this->OnResourcesLoaded();
        }
        return true;
    }
    else
    {
        // not all resources have been loaded yet
        return false;
    }
}

//------------------------------------------------------------------------------
/**
    This method is called once when all pending asynchronous resources have 
    been loaded (the Model is ready for rendering), this is when
    Model::GetResourceState() returns Loaded for the first time.
*/
void
Model::OnResourcesLoaded()
{
    n_assert(!this->resourcesLoaded);
    this->resourcesLoaded = true;
    IndexT i;
    for (i = 0; i < this->nodes.Size(); i++)
    {
        this->nodes[i]->OnResourcesLoaded();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Model::AttachNode(const Ptr<ModelNode>& node)
{
    n_assert(node->GetName().IsValid());
    this->nodes.Append(node);
    node->OnAttachToModel(this);
}

//------------------------------------------------------------------------------
/**
*/
void
Model::RemoveNode(const Ptr<ModelNode>& node)
{
    IndexT nodeIndex = this->nodes.FindIndex(node);
    n_assert(InvalidIndex != nodeIndex);
    node->OnRemoveFromModel();
    this->nodes.EraseIndex(nodeIndex);
}

//------------------------------------------------------------------------------
/**
*/
void
Model::AddVisibleModelNode(IndexT frameIndex, ModelNodeType::Code t, const Ptr<ModelNode>& modelNode)
{
    this->visibleModelNodes.Add(frameIndex, t, modelNode);
    if (!this->visibleModelNodes.IsResolved(t))
    {
        this->visibleModelNodes.SetResolved(t, true);
        VisResolver::Instance()->AddVisibleModel(frameIndex, t, this);
    }
}

//------------------------------------------------------------------------------
/**
*/
Ptr<ModelInstance>
Model::CreateInstance()
{
    // create a new ModelInstance from the root node
    Ptr<ModelInstance> modelInstance = ModelInstance::Create();
    modelInstance->Setup(this, this->GetRootNode());
    this->instances.Append(modelInstance);
    return modelInstance;
}

//------------------------------------------------------------------------------
/**
*/
Ptr<ModelInstance>
Model::CreatePartialInstance(const StringAtom& rootNodePath, const matrix44& rootNodeOffsetMatrix)
{
    // lookup the root node and create a new ModelInstance
    Ptr<ModelInstance> modelInstance = ModelInstance::Create();
    Ptr<ModelNode> treeRootNode = this->LookupNode(rootNodePath.AsString());
    n_assert(treeRootNode.isvalid());
    modelInstance->Setup(this, treeRootNode);
    this->instances.Append(modelInstance);

    // fixup the root node instance matrix with the offset matrix
    const Ptr<TransformNodeInstance>& rootNodeInst = modelInstance->GetRootNodeInstance().downcast<TransformNodeInstance>();
    rootNodeInst->SetOffsetMatrix(rootNodeOffsetMatrix);

    return modelInstance;
}

//------------------------------------------------------------------------------
/**
*/
void
Model::DiscardInstance(Ptr<ModelInstance> modelInst)
{
    IndexT modelInstIndex = this->instances.FindIndex(modelInst);
    n_assert(modelInstIndex != InvalidIndex);
    this->instances.EraseIndex(modelInstIndex);
    modelInst->Discard();
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
        else
        {
            // return an invalid ptr
            return Ptr<ModelNode>();
        }
    }
    return curPtr;
}

} // namespace Models
