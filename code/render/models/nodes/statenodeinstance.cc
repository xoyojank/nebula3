//------------------------------------------------------------------------------
//  statenodeinstance.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/nodes/statenodeinstance.h"
#include "models/nodes/statenode.h"
#include "coregraphics/shader.h"
#include "coregraphics/shadervariable.h"
#include "coregraphics/shadervariableinstance.h"
#include "coregraphics/transformdevice.h"

namespace Models
{
__ImplementClass(Models::StateNodeInstance, 'STNI', Models::TransformNodeInstance);

using namespace Util;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
StateNodeInstance::StateNodeInstance()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
StateNodeInstance::~StateNodeInstance()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
StateNodeInstance::Discard()
{
    this->shaderVariableInstances.Clear();
    TransformNodeInstance::Discard();
}

//------------------------------------------------------------------------------
/**
*/
void
StateNodeInstance::ApplyState()
{
    TransformNodeInstance::ApplyState();
 
    // apply the state of all shader variables instances
    IndexT i;
    for (i = 0; i < this->shaderVariableInstances.Size(); i++)
    {
        this->shaderVariableInstances.ValueAtIndex(i)->Apply();
    }

    // apply any needed model transform state to shader
    TransformDevice* transformDevice = TransformDevice::Instance();
    transformDevice->ApplyModelTransforms(this->modelNode.cast<StateNode>()->GetShaderInstance());
}

//------------------------------------------------------------------------------
/**
*/
Ptr<ShaderVariableInstance>
StateNodeInstance::CreateShaderVariableInstance(const ShaderVariable::Semantic& sem)
{
    n_assert(!this->shaderVariableInstances.Contains(sem));

    // get the shader from my StateNode
    n_assert(this->modelNode->IsA(StateNode::RTTI));
    const Ptr<ShaderInstance>& shaderInstance = this->modelNode.downcast<StateNode>()->GetShaderInstance();
    n_assert(shaderInstance.isvalid());
    
    // create new shader variable instance
    n_assert(shaderInstance->HasVariableBySemantic(sem));
    const Ptr<ShaderVariable>& var = shaderInstance->GetVariableBySemantic(sem);
    Ptr<ShaderVariableInstance> varInst = var->CreateInstance();
    this->shaderVariableInstances.Add(sem, varInst);
    return varInst;
}

//------------------------------------------------------------------------------
/**
*/
bool
StateNodeInstance::HasShaderVariableInstance(const ShaderVariable::Semantic& sem) const
{
    return this->shaderVariableInstances.Contains(sem);
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<ShaderVariableInstance>&
StateNodeInstance::GetShaderVariableInstance(const ShaderVariable::Semantic& sem) const
{
    n_assert(this->shaderVariableInstances.Contains(sem));
    return this->shaderVariableInstances[sem];
}

} // namespace Models
