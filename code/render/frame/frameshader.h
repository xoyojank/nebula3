#pragma once
//------------------------------------------------------------------------------
/**
    @class Frame::FrameShader
    
    A FrameShader controls the rendering of an entire frame, and is
    configured by an XML file.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "util/dictionary.h"
#include "resources/resourceid.h"
#include "resources/managedtexture.h"
#include "coregraphics/rendertarget.h"
#include "coregraphics/shadervariableinstance.h"
#include "frame/framepassbase.h"
#include "internalgraphics/internalcameraentity.h"

//------------------------------------------------------------------------------
namespace Frame
{
class FrameShader : public Core::RefCounted
{
    __DeclareClass(FrameShader);
public:
    /// constructor
    FrameShader();
    /// destructor
    virtual ~FrameShader();    
    /// discard the frame shader
    void Discard();
    /// render the frame shader from the given camera
    void Render();

    /// set the name of the frame shader
    void SetName(const Resources::ResourceId& id);
    /// get the name of the frame shader
    const Resources::ResourceId& GetName() const;
    /// set the main render target
    void SetMainRenderTarget(const Ptr<CoreGraphics::RenderTarget>& rt);
    /// get the main render target
    const Ptr<CoreGraphics::RenderTarget>& GetMainRenderTarget() const;

    /// add a render target to the frame
    void AddRenderTarget(const Resources::ResourceId& resId, const Ptr<CoreGraphics::RenderTarget>& rt);
    /// get number of render targets
    SizeT GetNumRenderTargets() const;
    /// get render target by index
    const Ptr<CoreGraphics::RenderTarget>& GetRenderTargetByIndex(IndexT i) const;
    /// return true if render target exists by name
    bool HasRenderTarget(const Resources::ResourceId& resId) const;
    /// get render target by name
    const Ptr<CoreGraphics::RenderTarget>& GetRenderTargetByName(const Resources::ResourceId& resId) const;

    /// add a mutliple render target to the frame
    void AddMultipleRenderTarget(const Resources::ResourceId& resId, const Ptr<CoreGraphics::MultipleRenderTarget>& rt);
    /// get number of mutliple render targets
    SizeT GetNumMultipleRenderTargets() const;
    /// get mutliple render target by index
    const Ptr<CoreGraphics::MultipleRenderTarget>& GetMultipleRenderTargetByIndex(IndexT i) const;
    /// return true if mutliple render target exists by name
    bool HasMultipleRenderTarget(const Resources::ResourceId& resId) const;
    /// get mutliple render target by name
    const Ptr<CoreGraphics::MultipleRenderTarget>& GetMultipleRenderTargetByName(const Resources::ResourceId& resId) const;

    /// add a texture to the frame
    void AddTexture(const Resources::ResourceId& resId, const Ptr<Resources::ManagedTexture>& t);
    /// get number of textures
    SizeT GetNumTextures() const;
    /// get texture by index
    const Ptr<Resources::ManagedTexture>& GetTextureByIndex(IndexT i) const;
    /// return true if texture exists by name
    bool HasTexture(const Resources::ResourceId& resId) const;
    /// get texture by name
    const Ptr<Resources::ManagedTexture>& GetTextureByName(const Resources::ResourceId& resId) const;

    /// add a frame pass to the frame shader
    void AddFramePassBase(const Ptr<FramePassBase>& framePass);
    /// get number of frame passes
    SizeT GetNumFramePassBases() const;
    /// get frame pass by index
    const Ptr<FramePassBase>& GetFramePassBaseByIndex(IndexT i) const;
    /// return true if names pass exists
    bool HasFramePassBase(const Resources::ResourceId& resId) const;
    /// get frame pass by name
    const Ptr<FramePassBase>& GetFramePassBaseByName(const Resources::ResourceId& resId) const;

private:
    Resources::ResourceId name;
    Ptr<CoreGraphics::RenderTarget> mainRenderTarget;
    Util::Dictionary<Resources::ResourceId, Ptr<CoreGraphics::RenderTarget> > renderTargets;
    Util::Dictionary<Resources::ResourceId, Ptr<CoreGraphics::MultipleRenderTarget> > multipleRenderTargets;
    Util::Dictionary<Resources::ResourceId, Ptr<Resources::ManagedTexture> > textures;
    Util::Dictionary<Resources::ResourceId, Ptr<CoreGraphics::ShaderVariableInstance> > shaderVariables;

    Util::Array<Ptr<FramePassBase> > framePasses;
    Util::Dictionary<Resources::ResourceId, IndexT> framePassIndexMap;
};

//------------------------------------------------------------------------------
/**
*/
inline void
FrameShader::SetName(const Resources::ResourceId& resId)
{
    this->name = resId;
}

//------------------------------------------------------------------------------
/**
*/
inline const Resources::ResourceId&
FrameShader::GetName() const
{
    return this->name;
}

//------------------------------------------------------------------------------
/**
*/
inline void
FrameShader::SetMainRenderTarget(const Ptr<CoreGraphics::RenderTarget>& rt)
{
    this->mainRenderTarget = rt;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::RenderTarget>&
FrameShader::GetMainRenderTarget() const
{
    return this->mainRenderTarget;
}

//------------------------------------------------------------------------------
/**
*/
inline void
FrameShader::AddRenderTarget(const Resources::ResourceId& resId, const Ptr<CoreGraphics::RenderTarget>& rt)
{
    n_assert(!this->renderTargets.Contains(resId));
    this->renderTargets.Add(resId, rt);
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
FrameShader::GetNumRenderTargets() const
{
    return this->renderTargets.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::RenderTarget>& 
FrameShader::GetRenderTargetByIndex(IndexT i) const
{
    return this->renderTargets.ValueAtIndex(i);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
FrameShader::HasRenderTarget(const Resources::ResourceId& resId) const
{
    return this->renderTargets.Contains(resId);
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::RenderTarget>&
FrameShader::GetRenderTargetByName(const Resources::ResourceId& resId) const
{
    return this->renderTargets[resId];
}

//------------------------------------------------------------------------------
/**
*/
inline void
FrameShader::AddMultipleRenderTarget(const Resources::ResourceId& resId, const Ptr<CoreGraphics::MultipleRenderTarget>& rt)
{
    n_assert(!this->multipleRenderTargets.Contains(resId));
    this->multipleRenderTargets.Add(resId, rt);
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
FrameShader::GetNumMultipleRenderTargets() const
{
    return this->multipleRenderTargets.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::MultipleRenderTarget>& 
FrameShader::GetMultipleRenderTargetByIndex(IndexT i) const
{
    return this->multipleRenderTargets.ValueAtIndex(i);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
FrameShader::HasMultipleRenderTarget(const Resources::ResourceId& resId) const
{
    return this->multipleRenderTargets.Contains(resId);
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::MultipleRenderTarget>&
FrameShader::GetMultipleRenderTargetByName(const Resources::ResourceId& resId) const
{
    return this->multipleRenderTargets[resId];
}
  
//------------------------------------------------------------------------------
/**
*/
inline void
FrameShader::AddTexture(const Resources::ResourceId& resId, const Ptr<Resources::ManagedTexture>& t)
{
    n_assert(!this->textures.Contains(resId));
    this->textures.Add(resId, t);
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
FrameShader::GetNumTextures() const
{
    return this->textures.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Resources::ManagedTexture>& 
FrameShader::GetTextureByIndex(IndexT i) const
{
    return this->textures.ValueAtIndex(i);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
FrameShader::HasTexture(const Resources::ResourceId& resId) const
{
    return this->textures.Contains(resId);
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Resources::ManagedTexture>&
FrameShader::GetTextureByName(const Resources::ResourceId& resId) const
{
    return this->textures[resId];
}

//------------------------------------------------------------------------------
/**
*/
inline void
FrameShader::AddFramePassBase(const Ptr<FramePassBase>& framePass)
{
    this->framePasses.Append(framePass);
    this->framePassIndexMap.Add(framePass->GetName(), this->framePasses.Size() - 1);
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
FrameShader::GetNumFramePassBases() const
{
    return this->framePasses.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<FramePassBase>&
FrameShader::GetFramePassBaseByIndex(IndexT i) const
{
    return this->framePasses[i];
}

//------------------------------------------------------------------------------
/**
*/
inline bool
FrameShader::HasFramePassBase(const Resources::ResourceId& resId) const
{
    return this->framePassIndexMap.Contains(resId);
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<FramePassBase>&
FrameShader::GetFramePassBaseByName(const Resources::ResourceId& resId) const
{
    return this->framePasses[this->framePassIndexMap[resId]];
}

} // namespace Frame
//------------------------------------------------------------------------------
