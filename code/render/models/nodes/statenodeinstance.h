#pragma once
//------------------------------------------------------------------------------
/**
    @class Models::StateNodeInstance

    Holds and applies per-instance shader state.
    
    (C) 2007 Radon Labs GmbH
*/
#include "models/nodes/transformnodeinstance.h"
#include "coregraphics/shadervariable.h"
#include "coregraphics/shadervariableinstance.h"

//------------------------------------------------------------------------------
namespace Models
{
class StateNodeInstance : public TransformNodeInstance
{
    __DeclareClass(StateNodeInstance);
public:
    /// constructor
    StateNodeInstance();
    /// destructor
    virtual ~StateNodeInstance();

    /// apply per-instance state prior to rendering
    virtual void ApplyState();

    /// instanciate a shader variable by semantic
    Ptr<CoreGraphics::ShaderVariableInstance> CreateShaderVariableInstance(const CoreGraphics::ShaderVariable::Semantic& semantic);
    /// return true if a shader variable has been instantiated
    bool HasShaderVariableInstance(const CoreGraphics::ShaderVariable::Semantic& semantic) const;
    /// get a shader variable instance
    const Ptr<CoreGraphics::ShaderVariableInstance>& GetShaderVariableInstance(const CoreGraphics::ShaderVariable::Semantic& semantic) const;

protected:
    /// called when removed from ModelInstance
    virtual void Discard();

    Util::Dictionary<CoreGraphics::ShaderVariable::Semantic, Ptr<CoreGraphics::ShaderVariableInstance> > shaderVariableInstances;
};

} // namespace Models
//------------------------------------------------------------------------------
