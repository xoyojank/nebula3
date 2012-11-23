//------------------------------------------------------------------------------
//  rtplugin.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "rendermodules/rt/rtplugin.h"

namespace RenderModules
{
__ImplementClass(RenderModules::RTPlugin, 'RTPG', Core::RefCounted);

using namespace InternalGraphics;

//------------------------------------------------------------------------------
/**
*/
RTPlugin::RTPlugin()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
RTPlugin::~RTPlugin()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
RTPlugin::OnRegister()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
RTPlugin::OnUnregister()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
RTPlugin::OnStageCreated(const Ptr<InternalStage>& stage)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
RTPlugin::OnDiscardStage(const Ptr<InternalStage>& stage)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
RTPlugin::OnViewCreated(const Ptr<InternalView>& view)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
RTPlugin::OnDiscardView(const Ptr<InternalView>& view)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
RTPlugin::OnAttachEntity(const Ptr<InternalGraphicsEntity>& entity)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
RTPlugin::OnRemoveEntity(const Ptr<InternalGraphicsEntity>& entity)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
RTPlugin::OnUpdateBefore(IndexT frameId, Timing::Time time)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
RTPlugin::OnUpdateAfter(IndexT frameId, Timing::Time time)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
RTPlugin::OnRenderBefore(IndexT frameId, Timing::Time time)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
RTPlugin::OnRenderAfter(IndexT frameId, Timing::Time time)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
RTPlugin::OnRenderFrameBatch(const Ptr<Frame::FrameBatch>& frameBatch)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
RTPlugin::OnRenderWithoutView(IndexT frameId, Timing::Time time)
{
    // empty
}

} // namespace RenderModules