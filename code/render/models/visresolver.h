#pragma once
//------------------------------------------------------------------------------
/**
    @class Models::VisResolver
  
    The VisResolver accepts visible ModelInstances and resolves
    them into their ModelNodeInstances, organized into node type
    and sorted for optimal rendering (instances of the same ModelNode
    should be rendered together to reduce state switch overhead).
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "models/visresolvecontainer.h"

//------------------------------------------------------------------------------
namespace Models
{
class ModelInstance;
class Model;
class ModelNode;
class ModelNodeInstance;

class VisResolver : public Core::RefCounted
{
    __DeclareClass(VisResolver);
    __DeclareSingleton(VisResolver);
public:
    /// constructor
    VisResolver();
    /// destructor
    virtual ~VisResolver();

    /// open the visibility resolver
    void Open();
    /// close the visibility resolver
    void Close();
    /// return true if currently open
    bool IsOpen() const;

    /// begin resolving 
    void BeginResolve(const Math::matrix44& cameraTransform);
    /// attach a visible ModelInstance
    void AttachVisibleModelInstance(const Ptr<ModelInstance>& inst);
    /// attach a visible ModelInstance
    void AttachVisibleModelInstancePlayerCamera(const Ptr<ModelInstance>& inst);
    /// end resolve
    void EndResolve();

    /// post-resolve: get Models with visible ModelNodeInstances by node type
    const Util::Array<Ptr<Model> >& GetVisibleModels(ModelNodeType::Code nodeType) const;
    /// post-resolve: get visible ModelNodes of a Model by node type
    const Util::Array<Ptr<ModelNode> >& GetVisibleModelNodes(ModelNodeType::Code nodeType, const Ptr<Model>& model) const;
    /// post-resolve: get visible ModelNodeInstance of a ModelNode by node type
    const Util::Array<Ptr<ModelNodeInstance> >& GetVisibleModelNodeInstances(ModelNodeType::Code nodeType, const Ptr<ModelNode>& modelNode) const;

private:
    friend class Model;

    /// add a visible model by node type
    void AddVisibleModel(IndexT frameIndex, ModelNodeType::Code nodeType, const Ptr<Model>& model);

    VisResolveContainer<Model> visibleModels;
    Math::matrix44 cameraTransform;
    IndexT resolveCount;
    bool isOpen;
    bool inResolve;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
VisResolver::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
inline void
VisResolver::AddVisibleModel(IndexT frameIndex, ModelNodeType::Code nodeType, const Ptr<Model>& model)
{
    n_assert(this->inResolve);
    this->visibleModels.Add(frameIndex, nodeType, model);
}

} // namespace Models
//------------------------------------------------------------------------------
