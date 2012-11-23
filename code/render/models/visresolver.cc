//------------------------------------------------------------------------------
//  visresolver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/visresolver.h"
#include "models/model.h"
#include "models/modelnode.h"
#include "models/modelnodeinstance.h"
#include "models/modelinstance.h"
#include "framesync/framesynctimer.h"

namespace Models
{
__ImplementClass(Models::VisResolver, 'VSRV', Core::RefCounted);
__ImplementSingleton(Models::VisResolver);

using namespace Util;
using namespace FrameSync;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
VisResolver::VisResolver() :
    cameraTransform(matrix44::identity()),
    resolveCount(InvalidIndex),
    isOpen(false),
    inResolve(false)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
VisResolver::~VisResolver()
{
    n_assert(!this->isOpen);
    n_assert(!this->inResolve);
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
VisResolver::Open()
{
    n_assert(!this->isOpen);
    n_assert(!this->inResolve);
    this->isOpen = true;
    this->resolveCount = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
VisResolver::Close()
{
    n_assert(this->isOpen);
    n_assert(!this->inResolve);
    this->visibleModels.Reset();
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void
VisResolver::BeginResolve(const matrix44& camTransform)
{
    n_assert(this->isOpen);
    n_assert(!this->inResolve);
    this->inResolve = true;
    this->cameraTransform = camTransform;
    this->resolveCount++;
    this->visibleModels.Reset();
}

//------------------------------------------------------------------------------
/**
*/
void
VisResolver::AttachVisibleModelInstance(const Ptr<ModelInstance>& inst)
{
    inst->OnVisibilityResolve(this->resolveCount, this->cameraTransform);
}

//------------------------------------------------------------------------------
/**
*/
void
VisResolver::AttachVisibleModelInstancePlayerCamera(const Ptr<ModelInstance>& inst)
{
    this->AttachVisibleModelInstance(inst);
    inst->UpdateRenderStats(this->cameraTransform);
}

//------------------------------------------------------------------------------
/**
*/
void
VisResolver::EndResolve()
{
    n_assert(this->inResolve);
    this->inResolve = false;
}

//------------------------------------------------------------------------------
/**
*/
const Array<Ptr<Model> >&
VisResolver::GetVisibleModels(ModelNodeType::Code nodeType) const
{
    n_assert(!this->inResolve);
    return this->visibleModels.Get(nodeType);
}

//------------------------------------------------------------------------------
/**
*/
const Array<Ptr<ModelNode> >&
VisResolver::GetVisibleModelNodes(ModelNodeType::Code nodeType, const Ptr<Model>& model) const
{
    return model->GetVisibleModelNodes(nodeType);
}

//------------------------------------------------------------------------------
/**
*/
const Array<Ptr<ModelNodeInstance> >&
VisResolver::GetVisibleModelNodeInstances(ModelNodeType::Code nodeType, const Ptr<ModelNode>& modelNode) const
{
    return modelNode->GetVisibleModelNodeInstances(nodeType);
}

} // namespace Models