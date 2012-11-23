#pragma once
//------------------------------------------------------------------------------
/**
    @class Base::ShaderBase
  
    A shader object groups render states required to render a piece of
    geometry. Shader objects are not used for rendering directly,
    instead ShaderInstances are created from a shader.

    (C) 2007 Radon Labs GmbH
*/    
#include "resources/resource.h"

namespace CoreGraphics
{
class ShaderInstance;
};

//------------------------------------------------------------------------------
namespace Base
{
class ShaderBase : public Resources::Resource
{
    __DeclareClass(ShaderBase);
public:
    /// constructor
    ShaderBase();
    /// destructor
    virtual ~ShaderBase();
    /// create a shader instance from this shader
    Ptr<CoreGraphics::ShaderInstance> CreateShaderInstance();
    /// discard a shader instance
    void DiscardShaderInstance(const Ptr<CoreGraphics::ShaderInstance>& inst);
    /// get all instances
    const Util::Array<Ptr<CoreGraphics::ShaderInstance> >& GetAllShaderInstances() const;

protected:
    Util::Array<Ptr<CoreGraphics::ShaderInstance> > shaderInstances;
};

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<CoreGraphics::ShaderInstance> >&
ShaderBase::GetAllShaderInstances() const
{
    return this->shaderInstances;
}

} // namespace Base
//------------------------------------------------------------------------------

