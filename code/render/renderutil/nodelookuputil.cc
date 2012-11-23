//------------------------------------------------------------------------------
//  nodelookuputil.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "renderutil/nodelookuputil.h"
#include "internalgraphics/internalmodelentity.h"

namespace RenderUtil
{
using namespace Models;
using namespace InternalGraphics;

//------------------------------------------------------------------------------
/**
    FIXME: THESE METHODS ARE SLOW AS HELL!!!

    Utility function which searches a specified statenodeinstance in a 
    internalgraphicsentity.

    returns 0 if 
        - resource is not loaded and resourcecheck flag is enabled
        - node was not found
        - internalmodelentity is not active
        - model instanes is not valid
*/
Ptr<StateNodeInstance>
NodeLookupUtil::LookupStateNodeInstance(const Ptr<InternalModelEntity>& entity, 
                                        const Util::StringAtom& nodeName, 
                                        bool checkResourceState /* = true */)
{
    n_assert(entity->GetType() == InternalGraphics::InternalGraphicsEntityType::Model);

    Ptr<InternalModelEntity> modelEntity = entity.cast<InternalModelEntity>();
    // is model entity deleted, and msg out-of-date, return handled = true to remove msg from list
    if (!modelEntity->IsActive())
    {
        return Ptr<StateNodeInstance>(0);
    }

    // check resource state if set to
    if (checkResourceState && modelEntity->GetModelResourceState() != Resources::Resource::Loaded)
    {
        return Ptr<StateNodeInstance>(0);
    }

    // check model instance
    const Ptr<ModelInstance>& modelInst =  modelEntity->GetModelInstance();    
    if (!modelInst.isvalid())
    {
        return Ptr<StateNodeInstance>(0);
    }

    // lookup, cast and return
    const Ptr<ModelNodeInstance>& nodeInst = modelInst->LookupNodeInstance(nodeName.Value());
    n_assert(nodeInst->IsA(StateNodeInstance::RTTI));
    const Ptr<StateNodeInstance>& stateNodeInst = nodeInst.cast<StateNodeInstance>();
    return stateNodeInst;
}

//------------------------------------------------------------------------------
/**
    FIXME: THESE METHODS ARE SLOW AS HELL!!!

    Utility function which searches a specified animatornodeinstance in a 
    internalgraphicsentity.

    returns 0 if 
    - resource is not loaded and resourcecheck flag is enabled
    - node was not found
    - internalmodelentity is not active
    - model instanes is not valid
*/
Ptr<AnimatorNodeInstance>
NodeLookupUtil::LookupAnimatorNodeInstance(const Ptr<InternalModelEntity>& entity, 
                                           const Util::StringAtom& nodeName, 
                                           bool checkResourceState /* = true */)
{
    n_assert(entity->GetType() == InternalGraphics::InternalGraphicsEntityType::Model);

    Ptr<InternalModelEntity> modelEntity = entity.cast<InternalModelEntity>();
    // is modelentity deleted, and msg out-of-date, return handled = true to remove msg from list
    if (!modelEntity->IsActive())
    {
        return Ptr<AnimatorNodeInstance>(0);
    }

    // check resource state if set to
    if (checkResourceState && modelEntity->GetModelResourceState() != Resources::Resource::Loaded)
    {
        return Ptr<AnimatorNodeInstance>(0);
    }

    // check model instance
    const Ptr<ModelInstance>& modelInst =  modelEntity->GetModelInstance();    
    if (!modelInst.isvalid())
    {
        return Ptr<AnimatorNodeInstance>(0);
    }

    // lookup, cast and return
    const Ptr<ModelNodeInstance>& nodeInst = modelInst->LookupNodeInstance(nodeName.Value());
    n_assert(nodeInst->IsA(AnimatorNodeInstance::RTTI));
    const Ptr<AnimatorNodeInstance>& animatorNodeInst = nodeInst.cast<AnimatorNodeInstance>();
    return animatorNodeInst;
}
} // namespace RenderUtil
