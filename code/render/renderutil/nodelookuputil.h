#pragma once
//------------------------------------------------------------------------------
/**
    @class RenderUtil::NodeLookupUtil

    Helper class to find specific nodes and nodeinstances inside a internalgraphicsentity

    WARNING: this util uses SLOW methods, like 'ModelInstance::LookupNodeInstance',
             use it careful!!!

    (C) 2009 Radon Labs GmbH
*/
#include "util/stringatom.h"
#include "models/nodes/statenodeinstance.h"
#include "models/nodes/animatornodeinstance.h"
#include "internalgraphics/internalmodelentity.h"

//------------------------------------------------------------------------------
namespace RenderUtil
{
class NodeLookupUtil
{
public:
    /// find state node instance
    static Ptr<Models::StateNodeInstance> LookupStateNodeInstance(const Ptr<InternalGraphics::InternalModelEntity>& entity,
                                                                  const Util::StringAtom& nodeName, 
                                                                  bool checkResourceState = true);

    /// find state node instance
    static Ptr<Models::AnimatorNodeInstance> LookupAnimatorNodeInstance(const Ptr<InternalGraphics::InternalModelEntity>& entity,
                                                                        const Util::StringAtom& nodeName, 
                                                                        bool checkResourceState = true);
};

} // namespace RenderUtil
//------------------------------------------------------------------------------

