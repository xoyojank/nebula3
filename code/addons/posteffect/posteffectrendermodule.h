#pragma once
//------------------------------------------------------------------------------
/**
    @class PostEffect::PostEffectRenderModule

    The client-side object for the posteffect subsystem

    (C) 2009 Radon Labs GmbH
*/
#include "core/singleton.h"
#include "rendermodules/rendermodule.h"

namespace PostEffect
{
class PostEffectRenderModule : public RenderModules::RenderModule
{
    __DeclareClass(PostEffectRenderModule);
    __DeclareSingleton(PostEffectRenderModule);
    
public:
    /// Constructor
    PostEffectRenderModule();
    /// Destructor
    virtual ~PostEffectRenderModule();

    /// Setup the render module
    virtual void Setup();
    /// Discard the render module
    virtual void Discard();
};

} // namespace PostEffect