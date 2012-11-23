#pragma once
//------------------------------------------------------------------------------
/**
    @class RenderModules::RTPluginRegistry

    The central registry for render thread plugins.

    (C) 2009 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "rendermodules/rt/rtplugin.h"
#include "frame/framebatch.h"

//------------------------------------------------------------------------------
namespace RenderModules
{
class RTPluginRegistry : public Core::RefCounted
{
    __DeclareClass(RTPluginRegistry);
    __DeclareSingleton(RTPluginRegistry);
public:
    /// constructor
    RTPluginRegistry();
    /// destructor
    virtual ~RTPluginRegistry();

    /// setup the plugin registry
    void Setup();
    /// discard the plugin registry
    void Discard();
    /// return true if currently valid
    bool IsValid() const;

    /// register a render plugin
    void RegisterRTPlugin(const Core::Rtti* rtti);
    /// unregister a render plugin
    void UnregisterRTPlugin(const Core::Rtti* rtti);
    /// get all currently registered plugins
    const Util::Array<Ptr<RTPlugin> >& GetRTPlugins() const;

    /// called when a new stage has been created
    virtual void OnStageCreated(const Ptr<InternalGraphics::InternalStage>& stage);
    /// called when a stage is discarded
    virtual void OnDiscardStage(const Ptr<InternalGraphics::InternalStage>& stage);
    /// called when a new view has been created
    virtual void OnViewCreated(const Ptr<InternalGraphics::InternalView>& view);
    /// called when a view is being discarded
    virtual void OnDiscardView(const Ptr<InternalGraphics::InternalView>& view);
    /// called when a graphics entity has been registered with the InternalGraphicsServer
    virtual void OnAttachEntity(const Ptr<InternalGraphics::InternalGraphicsEntity>& entity);
    /// called when a graphics entity is being removed from a stage
    virtual void OnRemoveEntity(const Ptr<InternalGraphics::InternalGraphicsEntity>& entity);
    /// called before updating entities
    virtual void OnUpdateBefore(IndexT frameId, Timing::Time time);
    /// called after updating entities
    virtual void OnUpdateAfter(IndexT frameId, Timing::Time time);
    /// called before rendering entities
    virtual void OnRenderBefore(IndexT frameId, Timing::Time time);
    /// called after rendering entities
    virtual void OnRenderAfter(IndexT frameId, Timing::Time time);
    /// called from when rendering a frame batch
    virtual void OnRenderFrameBatch(const Ptr<Frame::FrameBatch>& frameBatch);
    /// called if no view exists, and no default camera is set in view
    virtual void OnRenderWithoutView(IndexT frameId, Timing::Time time);

private:
    /// find plugin index by rtti
    IndexT FindPlugin(const Core::Rtti* rtti);

    Util::Array<Ptr<RTPlugin> > plugins;
    bool isValid;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
RTPluginRegistry::IsValid() const
{
    return this->isValid;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<RTPlugin> >&
RTPluginRegistry::GetRTPlugins() const
{
    return this->plugins;
}

} // namespace RenderModules
//------------------------------------------------------------------------------


