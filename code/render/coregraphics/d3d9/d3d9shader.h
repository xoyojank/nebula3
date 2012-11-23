#pragma once
//------------------------------------------------------------------------------
/**
    @class Direct3D9::D3D9Shader
    
    D3D9 implementation of Shader.

    (C) 2007 Radon Labs GmbH
*/
#include "coregraphics/base/shaderbase.h"
#include "coregraphics/shaderinstance.h"
#include "coregraphics/d3d9/d3d9resource.h"

//------------------------------------------------------------------------------
namespace Direct3D9
{
class D3D9Shader : public Base::ShaderBase, public D3D9Resource
{
    __DeclareClass(D3D9Shader);
public:
    /// constructor
    D3D9Shader();
    /// destructor
    virtual ~D3D9Shader();
   
    /// unload the resource, or cancel the pending load
    virtual void Unload();
    /// get pointer to d3d effect
    ID3DXEffect* GetD3D9Effect() const;

private:
    friend class D3D9StreamShaderLoader;

    /// set d3d9 effect object
    void SetD3D9Effect(ID3DXEffect* ptr);

    /// called when d3d9 device is lost
    virtual void OnLostDevice();
    /// called when d3d9 device is reset
    virtual void OnResetDevice();

    ID3DXEffect* d3d9Effect;
};

//------------------------------------------------------------------------------
/**
*/
inline void
D3D9Shader::SetD3D9Effect(ID3DXEffect* ptr)
{
    n_assert(0 != ptr);
    n_assert(0 == this->d3d9Effect);
    this->d3d9Effect = ptr;

	this->AddToResourceEventHandler();
}

//------------------------------------------------------------------------------
/**
*/
inline ID3DXEffect*
D3D9Shader::GetD3D9Effect() const
{
    n_assert(0 != this->d3d9Effect);
    return this->d3d9Effect;
}

} // namespace Direct3D9
//------------------------------------------------------------------------------

    