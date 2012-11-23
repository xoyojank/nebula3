#pragma once
//------------------------------------------------------------------------------
/**
    @class Video::VideoRenderModule

    The central client-side object for the video system

    (C) 2009 Radon Labs GmbH
*/

#include "core/singleton.h"
#include "rendermodules/rendermodule.h"

namespace Video
{
class VideoRenderModule : public RenderModules::RenderModule
{
    __DeclareClass(VideoRenderModule);
    __DeclareSingleton(VideoRenderModule);
public:
    /// Constructor
    VideoRenderModule();
    /// Destructor
    virtual ~VideoRenderModule();
    
    /// Setup the render module
    virtual void Setup();
    /// Discard the render module
    virtual void Discard();

private:
};
} // namespace Video