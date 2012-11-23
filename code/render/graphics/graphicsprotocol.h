#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "util/stringatom.h"
#include "util/variant.h"
#include "core/rtti.h"
#include "coregraphics/displaymode.h"
#include "coregraphics/adapter.h"
#include "coregraphics/antialiasquality.h"
#include "coregraphics/pixelformat.h"
#include "coregraphics/adapterinfo.h"
#include "coregraphics/threadsafedisplayeventhandler.h"
#include "coregraphics/threadsaferendereventhandler.h"
#include "internalgraphics/attachmentserver.h"
#include "internalgraphics/internalgraphicsentity.h"
#include "visibility/visibilitysystems/visibilitysystembase.h"
#include "resources/resourceid.h"
#include "math/matrix44.h"
#include "math/float4.h"
#include "math/quaternion.h"
#include "math/clipstatus.h"
#include "threading/objectref.h"
#include "timing/time.h"
#include "animation/animeventhandlerbase.h"
#include "coregraphics/mousepointer.h"
#include "resources/resourcemapper.h"
#include "shared/graphics/camerasettings.h"
#include "animation/animjobenqueuemode.h"

//------------------------------------------------------------------------------
namespace Graphics
{
//------------------------------------------------------------------------------
class RegisterRTPlugin : public Messaging::Message
{
    __DeclareClass(RegisterRTPlugin);
    __DeclareMsgId;
public:
    RegisterRTPlugin() 
    { };
public:
    void SetType(const Core::Rtti* val)
    {
        n_assert(!this->handled);
        this->type = val;
    };
    const Core::Rtti* GetType() const
    {
        return this->type;
    };
private:
    const Core::Rtti* type;
};
//------------------------------------------------------------------------------
class UnregisterRTPlugin : public Messaging::Message
{
    __DeclareClass(UnregisterRTPlugin);
    __DeclareMsgId;
public:
    UnregisterRTPlugin() 
    { };
public:
    void SetType(const Core::Rtti* val)
    {
        n_assert(!this->handled);
        this->type = val;
    };
    const Core::Rtti* GetType() const
    {
        return this->type;
    };
private:
    const Core::Rtti* type;
};
//------------------------------------------------------------------------------
class SetupGraphics : public Messaging::Message
{
    __DeclareClass(SetupGraphics);
    __DeclareMsgId;
public:
    SetupGraphics() 
    { };
public:
    void SetAdapter(const CoreGraphics::Adapter::Code& val)
    {
        n_assert(!this->handled);
        this->adapter = val;
    };
    const CoreGraphics::Adapter::Code& GetAdapter() const
    {
        return this->adapter;
    };
private:
    CoreGraphics::Adapter::Code adapter;
public:
    void SetDisplayMode(const CoreGraphics::DisplayMode& val)
    {
        n_assert(!this->handled);
        this->displaymode = val;
    };
    const CoreGraphics::DisplayMode& GetDisplayMode() const
    {
        return this->displaymode;
    };
private:
    CoreGraphics::DisplayMode displaymode;
public:
    void SetAntiAliasQuality(const CoreGraphics::AntiAliasQuality::Code& val)
    {
        n_assert(!this->handled);
        this->antialiasquality = val;
    };
    const CoreGraphics::AntiAliasQuality::Code& GetAntiAliasQuality() const
    {
        return this->antialiasquality;
    };
private:
    CoreGraphics::AntiAliasQuality::Code antialiasquality;
public:
    void SetFullscreen(bool val)
    {
        n_assert(!this->handled);
        this->fullscreen = val;
    };
    bool GetFullscreen() const
    {
        return this->fullscreen;
    };
private:
    bool fullscreen;
public:
    void SetAutoAdjustSize(bool val)
    {
        n_assert(!this->handled);
        this->autoadjustsize = val;
    };
    bool GetAutoAdjustSize() const
    {
        return this->autoadjustsize;
    };
private:
    bool autoadjustsize;
public:
    void SetDisplayModeSwitchEnabled(bool val)
    {
        n_assert(!this->handled);
        this->displaymodeswitchenabled = val;
    };
    bool GetDisplayModeSwitchEnabled() const
    {
        return this->displaymodeswitchenabled;
    };
private:
    bool displaymodeswitchenabled;
public:
    void SetTripleBufferingEnabled(bool val)
    {
        n_assert(!this->handled);
        this->triplebufferingenabled = val;
    };
    bool GetTripleBufferingEnabled() const
    {
        return this->triplebufferingenabled;
    };
private:
    bool triplebufferingenabled;
public:
    void SetAlwaysOnTop(bool val)
    {
        n_assert(!this->handled);
        this->alwaysontop = val;
    };
    bool GetAlwaysOnTop() const
    {
        return this->alwaysontop;
    };
private:
    bool alwaysontop;
public:
    void SetVerticalSyncEnabled(bool val)
    {
        n_assert(!this->handled);
        this->verticalsyncenabled = val;
    };
    bool GetVerticalSyncEnabled() const
    {
        return this->verticalsyncenabled;
    };
private:
    bool verticalsyncenabled;
public:
    void SetIconName(const Util::String& val)
    {
        n_assert(!this->handled);
        this->iconname = val;
    };
    const Util::String& GetIconName() const
    {
        return this->iconname;
    };
private:
    Util::String iconname;
public:
    void SetWindowTitle(const Util::String& val)
    {
        n_assert(!this->handled);
        this->windowtitle = val;
    };
    const Util::String& GetWindowTitle() const
    {
        return this->windowtitle;
    };
private:
    Util::String windowtitle;
public:
    void SetParentWindow(void* val)
    {
        n_assert(!this->handled);
        this->parentwindow = val;
    };
    void* GetParentWindow() const
    {
        return this->parentwindow;
    };
private:
    void* parentwindow;
public:
    void SetResourceMappers(const Util::Array<Ptr<Resources::ResourceMapper> >& val)
    {
        n_assert(!this->handled);
        this->resourcemappers = val;
    };
    const Util::Array<Ptr<Resources::ResourceMapper> >& GetResourceMappers() const
    {
        return this->resourcemappers;
    };
private:
    Util::Array<Ptr<Resources::ResourceMapper> > resourcemappers;
public:
    void SetSuccess(bool val)
    {
        n_assert(!this->handled);
        this->success = val;
    };
    bool GetSuccess() const
    {
        n_assert(this->handled);
        return this->success;
    };
private:
    bool success;
public:
    void SetActualDisplayMode(const CoreGraphics::DisplayMode& val)
    {
        n_assert(!this->handled);
        this->actualdisplaymode = val;
    };
    const CoreGraphics::DisplayMode& GetActualDisplayMode() const
    {
        n_assert(this->handled);
        return this->actualdisplaymode;
    };
private:
    CoreGraphics::DisplayMode actualdisplaymode;
public:
    void SetActualFullscreen(bool val)
    {
        n_assert(!this->handled);
        this->actualfullscreen = val;
    };
    bool GetActualFullscreen() const
    {
        n_assert(this->handled);
        return this->actualfullscreen;
    };
private:
    bool actualfullscreen;
public:
    void SetActualAdapter(const CoreGraphics::Adapter::Code& val)
    {
        n_assert(!this->handled);
        this->actualadapter = val;
    };
    const CoreGraphics::Adapter::Code& GetActualAdapter() const
    {
        n_assert(this->handled);
        return this->actualadapter;
    };
private:
    CoreGraphics::Adapter::Code actualadapter;
};
//------------------------------------------------------------------------------
class AdapterExists : public Messaging::Message
{
    __DeclareClass(AdapterExists);
    __DeclareMsgId;
public:
    AdapterExists() 
    { };
public:
    void SetAdapter(const CoreGraphics::Adapter::Code& val)
    {
        n_assert(!this->handled);
        this->adapter = val;
    };
    const CoreGraphics::Adapter::Code& GetAdapter() const
    {
        return this->adapter;
    };
private:
    CoreGraphics::Adapter::Code adapter;
public:
    void SetResult(bool val)
    {
        n_assert(!this->handled);
        this->result = val;
    };
    bool GetResult() const
    {
        n_assert(this->handled);
        return this->result;
    };
private:
    bool result;
};
//------------------------------------------------------------------------------
class GetAvailableDisplayModes : public Messaging::Message
{
    __DeclareClass(GetAvailableDisplayModes);
    __DeclareMsgId;
public:
    GetAvailableDisplayModes() 
    { };
public:
    void SetAdapter(const CoreGraphics::Adapter::Code& val)
    {
        n_assert(!this->handled);
        this->adapter = val;
    };
    const CoreGraphics::Adapter::Code& GetAdapter() const
    {
        return this->adapter;
    };
private:
    CoreGraphics::Adapter::Code adapter;
public:
    void SetPixelFormat(const CoreGraphics::PixelFormat::Code& val)
    {
        n_assert(!this->handled);
        this->pixelformat = val;
    };
    const CoreGraphics::PixelFormat::Code& GetPixelFormat() const
    {
        return this->pixelformat;
    };
private:
    CoreGraphics::PixelFormat::Code pixelformat;
public:
    void SetResult(const Util::Array<CoreGraphics::DisplayMode>& val)
    {
        n_assert(!this->handled);
        this->result = val;
    };
    const Util::Array<CoreGraphics::DisplayMode>& GetResult() const
    {
        n_assert(this->handled);
        return this->result;
    };
private:
    Util::Array<CoreGraphics::DisplayMode> result;
};
//------------------------------------------------------------------------------
class SupportsDisplayMode : public Messaging::Message
{
    __DeclareClass(SupportsDisplayMode);
    __DeclareMsgId;
public:
    SupportsDisplayMode() 
    { };
public:
    void SetAdapter(const CoreGraphics::Adapter::Code& val)
    {
        n_assert(!this->handled);
        this->adapter = val;
    };
    const CoreGraphics::Adapter::Code& GetAdapter() const
    {
        return this->adapter;
    };
private:
    CoreGraphics::Adapter::Code adapter;
public:
    void SetDisplayMode(const CoreGraphics::DisplayMode& val)
    {
        n_assert(!this->handled);
        this->displaymode = val;
    };
    const CoreGraphics::DisplayMode& GetDisplayMode() const
    {
        return this->displaymode;
    };
private:
    CoreGraphics::DisplayMode displaymode;
public:
    void SetResult(bool val)
    {
        n_assert(!this->handled);
        this->result = val;
    };
    bool GetResult() const
    {
        n_assert(this->handled);
        return this->result;
    };
private:
    bool result;
};
//------------------------------------------------------------------------------
class AdjustDisplaySize : public Messaging::Message
{
    __DeclareClass(AdjustDisplaySize);
    __DeclareMsgId;
public:
    AdjustDisplaySize() 
    { };
};
//------------------------------------------------------------------------------
class GetCurrentAdapterDisplayMode : public Messaging::Message
{
    __DeclareClass(GetCurrentAdapterDisplayMode);
    __DeclareMsgId;
public:
    GetCurrentAdapterDisplayMode() 
    { };
public:
    void SetAdapter(const CoreGraphics::Adapter::Code& val)
    {
        n_assert(!this->handled);
        this->adapter = val;
    };
    const CoreGraphics::Adapter::Code& GetAdapter() const
    {
        return this->adapter;
    };
private:
    CoreGraphics::Adapter::Code adapter;
public:
    void SetResult(const CoreGraphics::DisplayMode& val)
    {
        n_assert(!this->handled);
        this->result = val;
    };
    const CoreGraphics::DisplayMode& GetResult() const
    {
        n_assert(this->handled);
        return this->result;
    };
private:
    CoreGraphics::DisplayMode result;
};
//------------------------------------------------------------------------------
class GetAdapterInfo : public Messaging::Message
{
    __DeclareClass(GetAdapterInfo);
    __DeclareMsgId;
public:
    GetAdapterInfo() 
    { };
public:
    void SetAdapter(const CoreGraphics::Adapter::Code& val)
    {
        n_assert(!this->handled);
        this->adapter = val;
    };
    const CoreGraphics::Adapter::Code& GetAdapter() const
    {
        return this->adapter;
    };
private:
    CoreGraphics::Adapter::Code adapter;
public:
    void SetResult(const CoreGraphics::AdapterInfo& val)
    {
        n_assert(!this->handled);
        this->result = val;
    };
    const CoreGraphics::AdapterInfo& GetResult() const
    {
        n_assert(this->handled);
        return this->result;
    };
private:
    CoreGraphics::AdapterInfo result;
};
//------------------------------------------------------------------------------
class AttachDisplayEventHandler : public Messaging::Message
{
    __DeclareClass(AttachDisplayEventHandler);
    __DeclareMsgId;
public:
    AttachDisplayEventHandler() 
    { };
public:
    void SetHandler(const Ptr<CoreGraphics::ThreadSafeDisplayEventHandler>& val)
    {
        n_assert(!this->handled);
        this->handler = val;
    };
    const Ptr<CoreGraphics::ThreadSafeDisplayEventHandler>& GetHandler() const
    {
        return this->handler;
    };
private:
    Ptr<CoreGraphics::ThreadSafeDisplayEventHandler> handler;
};
//------------------------------------------------------------------------------
class RemoveDisplayEventHandler : public Messaging::Message
{
    __DeclareClass(RemoveDisplayEventHandler);
    __DeclareMsgId;
public:
    RemoveDisplayEventHandler() 
    { };
public:
    void SetHandler(const Ptr<CoreGraphics::ThreadSafeDisplayEventHandler>& val)
    {
        n_assert(!this->handled);
        this->handler = val;
    };
    const Ptr<CoreGraphics::ThreadSafeDisplayEventHandler>& GetHandler() const
    {
        return this->handler;
    };
private:
    Ptr<CoreGraphics::ThreadSafeDisplayEventHandler> handler;
};
//------------------------------------------------------------------------------
class AttachRenderEventHandler : public Messaging::Message
{
    __DeclareClass(AttachRenderEventHandler);
    __DeclareMsgId;
public:
    AttachRenderEventHandler() 
    { };
public:
    void SetHandler(const Ptr<CoreGraphics::ThreadSafeRenderEventHandler>& val)
    {
        n_assert(!this->handled);
        this->handler = val;
    };
    const Ptr<CoreGraphics::ThreadSafeRenderEventHandler>& GetHandler() const
    {
        return this->handler;
    };
private:
    Ptr<CoreGraphics::ThreadSafeRenderEventHandler> handler;
};
//------------------------------------------------------------------------------
class RemoveRenderEventHandler : public Messaging::Message
{
    __DeclareClass(RemoveRenderEventHandler);
    __DeclareMsgId;
public:
    RemoveRenderEventHandler() 
    { };
public:
    void SetHandler(const Ptr<CoreGraphics::ThreadSafeRenderEventHandler>& val)
    {
        n_assert(!this->handled);
        this->handler = val;
    };
    const Ptr<CoreGraphics::ThreadSafeRenderEventHandler>& GetHandler() const
    {
        return this->handler;
    };
private:
    Ptr<CoreGraphics::ThreadSafeRenderEventHandler> handler;
};
//------------------------------------------------------------------------------
class CreateGraphicsStage : public Messaging::Message
{
    __DeclareClass(CreateGraphicsStage);
    __DeclareMsgId;
public:
    CreateGraphicsStage() 
    { };
public:
    void SetName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->name = val;
    };
    const Util::StringAtom& GetName() const
    {
        return this->name;
    };
private:
    Util::StringAtom name;
public:
    void SetVisibleSystems(const Util::Array<Ptr<Visibility::VisibilitySystemBase> >& val)
    {
        n_assert(!this->handled);
        this->visiblesystems = val;
    };
    const Util::Array<Ptr<Visibility::VisibilitySystemBase> >& GetVisibleSystems() const
    {
        return this->visiblesystems;
    };
private:
    Util::Array<Ptr<Visibility::VisibilitySystemBase> > visiblesystems;
public:
    void SetObjectRef(const Ptr<Threading::ObjectRef>& val)
    {
        n_assert(!this->handled);
        this->objectref = val;
    };
    const Ptr<Threading::ObjectRef>& GetObjectRef() const
    {
        return this->objectref;
    };
private:
    Ptr<Threading::ObjectRef> objectref;
};
//------------------------------------------------------------------------------
class DiscardGraphicsStage : public Messaging::Message
{
    __DeclareClass(DiscardGraphicsStage);
    __DeclareMsgId;
public:
    DiscardGraphicsStage() 
    { };
public:
    void SetObjectRef(const Ptr<Threading::ObjectRef>& val)
    {
        n_assert(!this->handled);
        this->objectref = val;
    };
    const Ptr<Threading::ObjectRef>& GetObjectRef() const
    {
        return this->objectref;
    };
private:
    Ptr<Threading::ObjectRef> objectref;
};
//------------------------------------------------------------------------------
class CreateGraphicsView : public Messaging::Message
{
    __DeclareClass(CreateGraphicsView);
    __DeclareMsgId;
public:
    CreateGraphicsView() :
        viewclass(0),
        defaultview(false)
    { };
public:
    void SetViewClass(const Core::Rtti* val)
    {
        n_assert(!this->handled);
        this->viewclass = val;
    };
    const Core::Rtti* GetViewClass() const
    {
        return this->viewclass;
    };
private:
    const Core::Rtti* viewclass;
public:
    void SetName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->name = val;
    };
    const Util::StringAtom& GetName() const
    {
        return this->name;
    };
private:
    Util::StringAtom name;
public:
    void SetStageName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->stagename = val;
    };
    const Util::StringAtom& GetStageName() const
    {
        return this->stagename;
    };
private:
    Util::StringAtom stagename;
public:
    void SetFrameShaderName(const Resources::ResourceId& val)
    {
        n_assert(!this->handled);
        this->frameshadername = val;
    };
    const Resources::ResourceId& GetFrameShaderName() const
    {
        return this->frameshadername;
    };
private:
    Resources::ResourceId frameshadername;
public:
    void SetDefaultView(bool val)
    {
        n_assert(!this->handled);
        this->defaultview = val;
    };
    bool GetDefaultView() const
    {
        return this->defaultview;
    };
private:
    bool defaultview;
public:
    void SetObjectRef(const Ptr<Threading::ObjectRef>& val)
    {
        n_assert(!this->handled);
        this->objectref = val;
    };
    const Ptr<Threading::ObjectRef>& GetObjectRef() const
    {
        return this->objectref;
    };
private:
    Ptr<Threading::ObjectRef> objectref;
};
//------------------------------------------------------------------------------
class DiscardGraphicsView : public Messaging::Message
{
    __DeclareClass(DiscardGraphicsView);
    __DeclareMsgId;
public:
    DiscardGraphicsView() 
    { };
public:
    void SetObjectRef(const Ptr<Threading::ObjectRef>& val)
    {
        n_assert(!this->handled);
        this->objectref = val;
    };
    const Ptr<Threading::ObjectRef>& GetObjectRef() const
    {
        return this->objectref;
    };
private:
    Ptr<Threading::ObjectRef> objectref;
};
//------------------------------------------------------------------------------
class SetDefaultGraphicsView : public Messaging::Message
{
    __DeclareClass(SetDefaultGraphicsView);
    __DeclareMsgId;
public:
    SetDefaultGraphicsView() 
    { };
public:
    void SetObjectRef(const Ptr<Threading::ObjectRef>& val)
    {
        n_assert(!this->handled);
        this->objectref = val;
    };
    const Ptr<Threading::ObjectRef>& GetObjectRef() const
    {
        return this->objectref;
    };
private:
    Ptr<Threading::ObjectRef> objectref;
};
//------------------------------------------------------------------------------
class CreateGraphicsEntity : public Messaging::Message
{
    __DeclareClass(CreateGraphicsEntity);
    __DeclareMsgId;
public:
    CreateGraphicsEntity() :
        visible(true)
    { };
public:
    void SetStageName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->stagename = val;
    };
    const Util::StringAtom& GetStageName() const
    {
        return this->stagename;
    };
private:
    Util::StringAtom stagename;
public:
    void SetTransform(const Math::matrix44& val)
    {
        n_assert(!this->handled);
        this->transform = val;
    };
    const Math::matrix44& GetTransform() const
    {
        return this->transform;
    };
private:
    Math::matrix44 transform;
public:
    void SetVisible(bool val)
    {
        n_assert(!this->handled);
        this->visible = val;
    };
    bool GetVisible() const
    {
        return this->visible;
    };
private:
    bool visible;
public:
    void SetObjectRef(const Ptr<Threading::ObjectRef>& val)
    {
        n_assert(!this->handled);
        this->objectref = val;
    };
    const Ptr<Threading::ObjectRef>& GetObjectRef() const
    {
        return this->objectref;
    };
private:
    Ptr<Threading::ObjectRef> objectref;
public:
    void SetSharedData(const Ptr<FrameSync::FrameSyncSharedData>& val)
    {
        n_assert(!this->handled);
        this->shareddata = val;
    };
    const Ptr<FrameSync::FrameSyncSharedData>& GetSharedData() const
    {
        return this->shareddata;
    };
private:
    Ptr<FrameSync::FrameSyncSharedData> shareddata;
};
//------------------------------------------------------------------------------
class CreateModelEntity : public CreateGraphicsEntity
{
    __DeclareClass(CreateModelEntity);
    __DeclareMsgId;
public:
    CreateModelEntity() :
        animdrivenmotionenabled(false),
        animeventsenabled(false),
        animeventsonlydominatingclip(true),
        charjointtrackingenabled(false)
    { };
public:
    void SetResourceId(const Resources::ResourceId& val)
    {
        n_assert(!this->handled);
        this->resourceid = val;
    };
    const Resources::ResourceId& GetResourceId() const
    {
        return this->resourceid;
    };
private:
    Resources::ResourceId resourceid;
public:
    void SetRootNodePath(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->rootnodepath = val;
    };
    const Util::StringAtom& GetRootNodePath() const
    {
        return this->rootnodepath;
    };
private:
    Util::StringAtom rootnodepath;
public:
    void SetRootNodeOffsetMatrix(const Math::matrix44& val)
    {
        n_assert(!this->handled);
        this->rootnodeoffsetmatrix = val;
    };
    const Math::matrix44& GetRootNodeOffsetMatrix() const
    {
        return this->rootnodeoffsetmatrix;
    };
private:
    Math::matrix44 rootnodeoffsetmatrix;
public:
    void SetAnimDrivenMotionEnabled(bool val)
    {
        n_assert(!this->handled);
        this->animdrivenmotionenabled = val;
    };
    bool GetAnimDrivenMotionEnabled() const
    {
        return this->animdrivenmotionenabled;
    };
private:
    bool animdrivenmotionenabled;
public:
    void SetAnimDrivenMotionJointName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->animdrivenmotionjointname = val;
    };
    const Util::StringAtom& GetAnimDrivenMotionJointName() const
    {
        return this->animdrivenmotionjointname;
    };
private:
    Util::StringAtom animdrivenmotionjointname;
public:
    void SetAnimEventsEnabled(bool val)
    {
        n_assert(!this->handled);
        this->animeventsenabled = val;
    };
    bool GetAnimEventsEnabled() const
    {
        return this->animeventsenabled;
    };
private:
    bool animeventsenabled;
public:
    void SetAnimEventsOnlyDominatingClip(bool val)
    {
        n_assert(!this->handled);
        this->animeventsonlydominatingclip = val;
    };
    bool GetAnimEventsOnlyDominatingClip() const
    {
        return this->animeventsonlydominatingclip;
    };
private:
    bool animeventsonlydominatingclip;
public:
    void SetCharJointTrackingEnabled(bool val)
    {
        n_assert(!this->handled);
        this->charjointtrackingenabled = val;
    };
    bool GetCharJointTrackingEnabled() const
    {
        return this->charjointtrackingenabled;
    };
private:
    bool charjointtrackingenabled;
public:
    void SetTrackedCharJointNames(const Util::Array<Util::StringAtom>& val)
    {
        n_assert(!this->handled);
        this->trackedcharjointnames = val;
    };
    const Util::Array<Util::StringAtom>& GetTrackedCharJointNames() const
    {
        return this->trackedcharjointnames;
    };
private:
    Util::Array<Util::StringAtom> trackedcharjointnames;
};
//------------------------------------------------------------------------------
class CreateCameraEntity : public CreateGraphicsEntity
{
    __DeclareClass(CreateCameraEntity);
    __DeclareMsgId;
public:
    CreateCameraEntity() 
    { };
public:
    void SetCameraSettings(const Shared::CameraSettings& val)
    {
        n_assert(!this->handled);
        this->camerasettings = val;
    };
    const Shared::CameraSettings& GetCameraSettings() const
    {
        return this->camerasettings;
    };
private:
    Shared::CameraSettings camerasettings;
};
//------------------------------------------------------------------------------
class CreateGlobalLightEntity : public CreateGraphicsEntity
{
    __DeclareClass(CreateGlobalLightEntity);
    __DeclareMsgId;
public:
    CreateGlobalLightEntity() 
    { };
public:
    void SetLightColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->lightcolor = val;
    };
    const Math::float4& GetLightColor() const
    {
        return this->lightcolor;
    };
private:
    Math::float4 lightcolor;
public:
    void SetProjMapUvOffsetAndScale(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->projmapuvoffsetandscale = val;
    };
    const Math::float4& GetProjMapUvOffsetAndScale() const
    {
        return this->projmapuvoffsetandscale;
    };
private:
    Math::float4 projmapuvoffsetandscale;
public:
    void SetCastShadows(bool val)
    {
        n_assert(!this->handled);
        this->castshadows = val;
    };
    bool GetCastShadows() const
    {
        return this->castshadows;
    };
private:
    bool castshadows;
public:
    void SetBackLightColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->backlightcolor = val;
    };
    const Math::float4& GetBackLightColor() const
    {
        return this->backlightcolor;
    };
private:
    Math::float4 backlightcolor;
public:
    void SetAmbientLightColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->ambientlightcolor = val;
    };
    const Math::float4& GetAmbientLightColor() const
    {
        return this->ambientlightcolor;
    };
private:
    Math::float4 ambientlightcolor;
public:
    void SetBackLightOffset(float val)
    {
        n_assert(!this->handled);
        this->backlightoffset = val;
    };
    float GetBackLightOffset() const
    {
        return this->backlightoffset;
    };
private:
    float backlightoffset;
};
//------------------------------------------------------------------------------
class CreateSpotLightEntity : public CreateGraphicsEntity
{
    __DeclareClass(CreateSpotLightEntity);
    __DeclareMsgId;
public:
    CreateSpotLightEntity() 
    { };
public:
    void SetLightColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->lightcolor = val;
    };
    const Math::float4& GetLightColor() const
    {
        return this->lightcolor;
    };
private:
    Math::float4 lightcolor;
public:
    void SetProjMapUvOffsetAndScale(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->projmapuvoffsetandscale = val;
    };
    const Math::float4& GetProjMapUvOffsetAndScale() const
    {
        return this->projmapuvoffsetandscale;
    };
private:
    Math::float4 projmapuvoffsetandscale;
public:
    void SetCastShadows(bool val)
    {
        n_assert(!this->handled);
        this->castshadows = val;
    };
    bool GetCastShadows() const
    {
        return this->castshadows;
    };
private:
    bool castshadows;
};
//------------------------------------------------------------------------------
class CreatePointLightEntity : public CreateGraphicsEntity
{
    __DeclareClass(CreatePointLightEntity);
    __DeclareMsgId;
public:
    CreatePointLightEntity() 
    { };
public:
    void SetLightColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->lightcolor = val;
    };
    const Math::float4& GetLightColor() const
    {
        return this->lightcolor;
    };
private:
    Math::float4 lightcolor;
public:
    void SetProjMapUvOffsetAndScale(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->projmapuvoffsetandscale = val;
    };
    const Math::float4& GetProjMapUvOffsetAndScale() const
    {
        return this->projmapuvoffsetandscale;
    };
private:
    Math::float4 projmapuvoffsetandscale;
public:
    void SetCastShadows(bool val)
    {
        n_assert(!this->handled);
        this->castshadows = val;
    };
    bool GetCastShadows() const
    {
        return this->castshadows;
    };
private:
    bool castshadows;
};
//------------------------------------------------------------------------------
class GraphicsEntityMessage : public Messaging::Message
{
    __DeclareClass(GraphicsEntityMessage);
    __DeclareMsgId;
public:
    GraphicsEntityMessage() 
    { };
public:
    void SetObjectRef(const Ptr<Threading::ObjectRef>& val)
    {
        n_assert(!this->handled);
        this->objectref = val;
    };
    const Ptr<Threading::ObjectRef>& GetObjectRef() const
    {
        return this->objectref;
    };
private:
    Ptr<Threading::ObjectRef> objectref;
};
//------------------------------------------------------------------------------
class DiscardGraphicsEntity : public GraphicsEntityMessage
{
    __DeclareClass(DiscardGraphicsEntity);
    __DeclareMsgId;
public:
    DiscardGraphicsEntity() 
    { };
};
//------------------------------------------------------------------------------
class AddTrackedCharJoint : public GraphicsEntityMessage
{
    __DeclareClass(AddTrackedCharJoint);
    __DeclareMsgId;
public:
    AddTrackedCharJoint() 
    { };
public:
    void SetJointName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->jointname = val;
    };
    const Util::StringAtom& GetJointName() const
    {
        return this->jointname;
    };
private:
    Util::StringAtom jointname;
};
//------------------------------------------------------------------------------
class SetLightColor : public GraphicsEntityMessage
{
    __DeclareClass(SetLightColor);
    __DeclareMsgId;
public:
    SetLightColor() 
    { };
public:
    void SetColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->color = val;
    };
    const Math::float4& GetColor() const
    {
        return this->color;
    };
private:
    Math::float4 color;
};
//------------------------------------------------------------------------------
class SetLightCastShadows : public GraphicsEntityMessage
{
    __DeclareClass(SetLightCastShadows);
    __DeclareMsgId;
public:
    SetLightCastShadows() 
    { };
public:
    void SetCastShadows(bool val)
    {
        n_assert(!this->handled);
        this->castshadows = val;
    };
    bool GetCastShadows() const
    {
        return this->castshadows;
    };
private:
    bool castshadows;
};
//------------------------------------------------------------------------------
class SetGlobalBackLightColor : public GraphicsEntityMessage
{
    __DeclareClass(SetGlobalBackLightColor);
    __DeclareMsgId;
public:
    SetGlobalBackLightColor() 
    { };
public:
    void SetColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->color = val;
    };
    const Math::float4& GetColor() const
    {
        return this->color;
    };
private:
    Math::float4 color;
};
//------------------------------------------------------------------------------
class SetGlobalAmbientLightColor : public GraphicsEntityMessage
{
    __DeclareClass(SetGlobalAmbientLightColor);
    __DeclareMsgId;
public:
    SetGlobalAmbientLightColor() 
    { };
public:
    void SetColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->color = val;
    };
    const Math::float4& GetColor() const
    {
        return this->color;
    };
private:
    Math::float4 color;
};
//------------------------------------------------------------------------------
class SetGlobalBackLightOffset : public GraphicsEntityMessage
{
    __DeclareClass(SetGlobalBackLightOffset);
    __DeclareMsgId;
public:
    SetGlobalBackLightOffset() 
    { };
public:
    void SetOffset(float val)
    {
        n_assert(!this->handled);
        this->offset = val;
    };
    float GetOffset() const
    {
        return this->offset;
    };
private:
    float offset;
};
//------------------------------------------------------------------------------
class SetGlobalLightParams : public GraphicsEntityMessage
{
    __DeclareClass(SetGlobalLightParams);
    __DeclareMsgId;
public:
    SetGlobalLightParams() 
    { };
public:
    void SetLightColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->lightcolor = val;
    };
    const Math::float4& GetLightColor() const
    {
        return this->lightcolor;
    };
private:
    Math::float4 lightcolor;
public:
    void SetBackLightColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->backlightcolor = val;
    };
    const Math::float4& GetBackLightColor() const
    {
        return this->backlightcolor;
    };
private:
    Math::float4 backlightcolor;
public:
    void SetAmbientLightColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->ambientlightcolor = val;
    };
    const Math::float4& GetAmbientLightColor() const
    {
        return this->ambientlightcolor;
    };
private:
    Math::float4 ambientlightcolor;
public:
    void SetBackLightOffset(float val)
    {
        n_assert(!this->handled);
        this->backlightoffset = val;
    };
    float GetBackLightOffset() const
    {
        return this->backlightoffset;
    };
private:
    float backlightoffset;
public:
    void SetCastShadows(bool val)
    {
        n_assert(!this->handled);
        this->castshadows = val;
    };
    bool GetCastShadows() const
    {
        return this->castshadows;
    };
private:
    bool castshadows;
};
//------------------------------------------------------------------------------
class SetGlobalLightColor : public GraphicsEntityMessage
{
    __DeclareClass(SetGlobalLightColor);
    __DeclareMsgId;
public:
    SetGlobalLightColor() 
    { };
public:
    void SetBackLightColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->backlightcolor = val;
    };
    const Math::float4& GetBackLightColor() const
    {
        return this->backlightcolor;
    };
private:
    Math::float4 backlightcolor;
public:
    void SetAmbientLightColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->ambientlightcolor = val;
    };
    const Math::float4& GetAmbientLightColor() const
    {
        return this->ambientlightcolor;
    };
private:
    Math::float4 ambientlightcolor;
public:
    void SetBackLightOffset(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->backlightoffset = val;
    };
    const Math::float4& GetBackLightOffset() const
    {
        return this->backlightoffset;
    };
private:
    Math::float4 backlightoffset;
};
//------------------------------------------------------------------------------
class UpdateTransform : public GraphicsEntityMessage
{
    __DeclareClass(UpdateTransform);
    __DeclareMsgId;
public:
    UpdateTransform() 
    { };
public:
    void SetTransform(const Math::matrix44& val)
    {
        n_assert(!this->handled);
        this->transform = val;
    };
    const Math::matrix44& GetTransform() const
    {
        return this->transform;
    };
private:
    Math::matrix44 transform;
};
//------------------------------------------------------------------------------
class SetVisibility : public GraphicsEntityMessage
{
    __DeclareClass(SetVisibility);
    __DeclareMsgId;
public:
    SetVisibility() 
    { };
public:
    void SetVisible(bool val)
    {
        n_assert(!this->handled);
        this->visible = val;
    };
    bool GetVisible() const
    {
        return this->visible;
    };
private:
    bool visible;
};
//------------------------------------------------------------------------------
class AttachCameraToView : public GraphicsEntityMessage
{
    __DeclareClass(AttachCameraToView);
    __DeclareMsgId;
public:
    AttachCameraToView() 
    { };
public:
    void SetViewName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->viewname = val;
    };
    const Util::StringAtom& GetViewName() const
    {
        return this->viewname;
    };
private:
    Util::StringAtom viewname;
};
//------------------------------------------------------------------------------
class RemoveCameraFromView : public GraphicsEntityMessage
{
    __DeclareClass(RemoveCameraFromView);
    __DeclareMsgId;
public:
    RemoveCameraFromView() 
    { };
public:
    void SetViewName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->viewname = val;
    };
    const Util::StringAtom& GetViewName() const
    {
        return this->viewname;
    };
private:
    Util::StringAtom viewname;
};
//------------------------------------------------------------------------------
class UpdModelNodeInstanceShaderVariable : public GraphicsEntityMessage
{
    __DeclareClass(UpdModelNodeInstanceShaderVariable);
    __DeclareMsgId;
public:
    UpdModelNodeInstanceShaderVariable() 
    { };
public:
    void SetModelNodeInstanceName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->modelnodeinstancename = val;
    };
    const Util::StringAtom& GetModelNodeInstanceName() const
    {
        return this->modelnodeinstancename;
    };
private:
    Util::StringAtom modelnodeinstancename;
public:
    void SetSemantic(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->semantic = val;
    };
    const Util::StringAtom& GetSemantic() const
    {
        return this->semantic;
    };
private:
    Util::StringAtom semantic;
public:
    void SetValue(const Util::Variant& val)
    {
        n_assert(!this->handled);
        this->value = val;
    };
    const Util::Variant& GetValue() const
    {
        return this->value;
    };
private:
    Util::Variant value;
};
//------------------------------------------------------------------------------
class UpdModelNodeInstanceVisibility : public GraphicsEntityMessage
{
    __DeclareClass(UpdModelNodeInstanceVisibility);
    __DeclareMsgId;
public:
    UpdModelNodeInstanceVisibility() 
    { };
public:
    void SetModelNodeInstanceName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->modelnodeinstancename = val;
    };
    const Util::StringAtom& GetModelNodeInstanceName() const
    {
        return this->modelnodeinstancename;
    };
private:
    Util::StringAtom modelnodeinstancename;
public:
    void SetVisible(bool val)
    {
        n_assert(!this->handled);
        this->visible = val;
    };
    bool GetVisible() const
    {
        return this->visible;
    };
private:
    bool visible;
};
//------------------------------------------------------------------------------
class UpdateSharedShaderVariable : public Messaging::Message
{
    __DeclareClass(UpdateSharedShaderVariable);
    __DeclareMsgId;
public:
    UpdateSharedShaderVariable() 
    { };
public:
    void SetSemantic(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->semantic = val;
    };
    const Util::StringAtom& GetSemantic() const
    {
        return this->semantic;
    };
private:
    Util::StringAtom semantic;
public:
    void SetValue(const Util::Variant& val)
    {
        n_assert(!this->handled);
        this->value = val;
    };
    const Util::Variant& GetValue() const
    {
        return this->value;
    };
private:
    Util::Variant value;
};
//------------------------------------------------------------------------------
class AnimPlayClip : public GraphicsEntityMessage
{
    __DeclareClass(AnimPlayClip);
    __DeclareMsgId;
public:
    AnimPlayClip() :
        trackindex(0),
        loopcount(1.0f),
        starttime(0),
        fadeintime(200),
        fadeouttime(200),
        timeoffset(0),
        timefactor(1.0f),
        blendweight(1.0f),
        exclusivetag(InvalidIndex),
        enqueuemode(Animation::AnimJobEnqueueMode::Intercept)
    { };
public:
    void SetClipName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->clipname = val;
    };
    const Util::StringAtom& GetClipName() const
    {
        return this->clipname;
    };
private:
    Util::StringAtom clipname;
public:
    void SetTrackIndex(IndexT val)
    {
        n_assert(!this->handled);
        this->trackindex = val;
    };
    IndexT GetTrackIndex() const
    {
        return this->trackindex;
    };
private:
    IndexT trackindex;
public:
    void SetLoopCount(float val)
    {
        n_assert(!this->handled);
        this->loopcount = val;
    };
    float GetLoopCount() const
    {
        return this->loopcount;
    };
private:
    float loopcount;
public:
    void SetStartTime(const Timing::Tick& val)
    {
        n_assert(!this->handled);
        this->starttime = val;
    };
    const Timing::Tick& GetStartTime() const
    {
        return this->starttime;
    };
private:
    Timing::Tick starttime;
public:
    void SetFadeInTime(const Timing::Tick& val)
    {
        n_assert(!this->handled);
        this->fadeintime = val;
    };
    const Timing::Tick& GetFadeInTime() const
    {
        return this->fadeintime;
    };
private:
    Timing::Tick fadeintime;
public:
    void SetFadeOutTime(const Timing::Tick& val)
    {
        n_assert(!this->handled);
        this->fadeouttime = val;
    };
    const Timing::Tick& GetFadeOutTime() const
    {
        return this->fadeouttime;
    };
private:
    Timing::Tick fadeouttime;
public:
    void SetTimeOffset(const Timing::Tick& val)
    {
        n_assert(!this->handled);
        this->timeoffset = val;
    };
    const Timing::Tick& GetTimeOffset() const
    {
        return this->timeoffset;
    };
private:
    Timing::Tick timeoffset;
public:
    void SetTimeFactor(float val)
    {
        n_assert(!this->handled);
        this->timefactor = val;
    };
    float GetTimeFactor() const
    {
        return this->timefactor;
    };
private:
    float timefactor;
public:
    void SetBlendWeight(float val)
    {
        n_assert(!this->handled);
        this->blendweight = val;
    };
    float GetBlendWeight() const
    {
        return this->blendweight;
    };
private:
    float blendweight;
public:
    void SetExclusiveTag(IndexT val)
    {
        n_assert(!this->handled);
        this->exclusivetag = val;
    };
    IndexT GetExclusiveTag() const
    {
        return this->exclusivetag;
    };
private:
    IndexT exclusivetag;
public:
    void SetEnqueueMode(const Animation::AnimJobEnqueueMode::Code& val)
    {
        n_assert(!this->handled);
        this->enqueuemode = val;
    };
    const Animation::AnimJobEnqueueMode::Code& GetEnqueueMode() const
    {
        return this->enqueuemode;
    };
private:
    Animation::AnimJobEnqueueMode::Code enqueuemode;
};
//------------------------------------------------------------------------------
class AnimStopTrack : public GraphicsEntityMessage
{
    __DeclareClass(AnimStopTrack);
    __DeclareMsgId;
public:
    AnimStopTrack() :
        trackindex(0),
        allowfadeout(true)
    { };
public:
    void SetTrackIndex(IndexT val)
    {
        n_assert(!this->handled);
        this->trackindex = val;
    };
    IndexT GetTrackIndex() const
    {
        return this->trackindex;
    };
private:
    IndexT trackindex;
public:
    void SetAllowFadeOut(bool val)
    {
        n_assert(!this->handled);
        this->allowfadeout = val;
    };
    bool GetAllowFadeOut() const
    {
        return this->allowfadeout;
    };
private:
    bool allowfadeout;
};
//------------------------------------------------------------------------------
class AnimStopAllTracks : public GraphicsEntityMessage
{
    __DeclareClass(AnimStopAllTracks);
    __DeclareMsgId;
public:
    AnimStopAllTracks() :
        allowfadeout(true)
    { };
public:
    void SetAllowFadeOut(bool val)
    {
        n_assert(!this->handled);
        this->allowfadeout = val;
    };
    bool GetAllowFadeOut() const
    {
        return this->allowfadeout;
    };
private:
    bool allowfadeout;
};
//------------------------------------------------------------------------------
class AnimModifyBlendWeight : public GraphicsEntityMessage
{
    __DeclareClass(AnimModifyBlendWeight);
    __DeclareMsgId;
public:
    AnimModifyBlendWeight() :
        trackindex(0),
        blendweight(1.0f)
    { };
public:
    void SetTrackIndex(IndexT val)
    {
        n_assert(!this->handled);
        this->trackindex = val;
    };
    IndexT GetTrackIndex() const
    {
        return this->trackindex;
    };
private:
    IndexT trackindex;
public:
    void SetBlendWeight(float val)
    {
        n_assert(!this->handled);
        this->blendweight = val;
    };
    float GetBlendWeight() const
    {
        return this->blendweight;
    };
private:
    float blendweight;
};
//------------------------------------------------------------------------------
class AnimModifyTimeFactor : public GraphicsEntityMessage
{
    __DeclareClass(AnimModifyTimeFactor);
    __DeclareMsgId;
public:
    AnimModifyTimeFactor() :
        trackindex(0),
        timefactor(1.0)
    { };
public:
    void SetTrackIndex(IndexT val)
    {
        n_assert(!this->handled);
        this->trackindex = val;
    };
    IndexT GetTrackIndex() const
    {
        return this->trackindex;
    };
private:
    IndexT trackindex;
public:
    void SetTimeFactor(float val)
    {
        n_assert(!this->handled);
        this->timefactor = val;
    };
    float GetTimeFactor() const
    {
        return this->timefactor;
    };
private:
    float timefactor;
};
//------------------------------------------------------------------------------
class AnimIsClipPlaying : public GraphicsEntityMessage
{
    __DeclareClass(AnimIsClipPlaying);
    __DeclareMsgId;
public:
    AnimIsClipPlaying() 
    { };
public:
    void SetClipName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->clipname = val;
    };
    const Util::StringAtom& GetClipName() const
    {
        return this->clipname;
    };
private:
    Util::StringAtom clipname;
public:
    void SetPlaying(bool val)
    {
        n_assert(!this->handled);
        this->playing = val;
    };
    bool GetPlaying() const
    {
        n_assert(this->handled);
        return this->playing;
    };
private:
    bool playing;
};
//------------------------------------------------------------------------------
class ApplySkinList : public GraphicsEntityMessage
{
    __DeclareClass(ApplySkinList);
    __DeclareMsgId;
public:
    ApplySkinList() 
    { };
public:
    void SetSkinList(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->skinlist = val;
    };
    const Util::StringAtom& GetSkinList() const
    {
        return this->skinlist;
    };
private:
    Util::StringAtom skinlist;
};
//------------------------------------------------------------------------------
class ShowSkin : public GraphicsEntityMessage
{
    __DeclareClass(ShowSkin);
    __DeclareMsgId;
public:
    ShowSkin() 
    { };
public:
    void SetSkin(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->skin = val;
    };
    const Util::StringAtom& GetSkin() const
    {
        return this->skin;
    };
private:
    Util::StringAtom skin;
};
//------------------------------------------------------------------------------
class HideSkin : public GraphicsEntityMessage
{
    __DeclareClass(HideSkin);
    __DeclareMsgId;
public:
    HideSkin() 
    { };
public:
    void SetSkin(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->skin = val;
    };
    const Util::StringAtom& GetSkin() const
    {
        return this->skin;
    };
private:
    Util::StringAtom skin;
};
//------------------------------------------------------------------------------
class SetVariation : public GraphicsEntityMessage
{
    __DeclareClass(SetVariation);
    __DeclareMsgId;
public:
    SetVariation() 
    { };
public:
    void SetVariationName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->variationname = val;
    };
    const Util::StringAtom& GetVariationName() const
    {
        return this->variationname;
    };
private:
    Util::StringAtom variationname;
};
//------------------------------------------------------------------------------
class SetTimeFactor : public Messaging::Message
{
    __DeclareClass(SetTimeFactor);
    __DeclareMsgId;
public:
    SetTimeFactor() 
    { };
public:
    void SetFactor(float val)
    {
        n_assert(!this->handled);
        this->factor = val;
    };
    float GetFactor() const
    {
        return this->factor;
    };
private:
    float factor;
};
//------------------------------------------------------------------------------
class UnbindAttachment : public Messaging::Message
{
    __DeclareClass(UnbindAttachment);
    __DeclareMsgId;
public:
    UnbindAttachment() :
        cleartype(InternalGraphics::AttachmentServer::ClearNone)
    { };
public:
    void SetBaseEntityObjectRef(const Ptr<Threading::ObjectRef>& val)
    {
        n_assert(!this->handled);
        this->baseentityobjectref = val;
    };
    const Ptr<Threading::ObjectRef>& GetBaseEntityObjectRef() const
    {
        return this->baseentityobjectref;
    };
private:
    Ptr<Threading::ObjectRef> baseentityobjectref;
public:
    void SetEntityToAttachObjectRef(const Ptr<Threading::ObjectRef>& val)
    {
        n_assert(!this->handled);
        this->entitytoattachobjectref = val;
    };
    const Ptr<Threading::ObjectRef>& GetEntityToAttachObjectRef() const
    {
        return this->entitytoattachobjectref;
    };
private:
    Ptr<Threading::ObjectRef> entitytoattachobjectref;
public:
    void SetJoint(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->joint = val;
    };
    const Util::StringAtom& GetJoint() const
    {
        return this->joint;
    };
private:
    Util::StringAtom joint;
public:
    void SetClearType(const InternalGraphics::AttachmentServer::ClearType& val)
    {
        n_assert(!this->handled);
        this->cleartype = val;
    };
    const InternalGraphics::AttachmentServer::ClearType& GetClearType() const
    {
        return this->cleartype;
    };
private:
    InternalGraphics::AttachmentServer::ClearType cleartype;
};
//------------------------------------------------------------------------------
class BindAttachment : public UnbindAttachment
{
    __DeclareClass(BindAttachment);
    __DeclareMsgId;
public:
    BindAttachment() 
    { };
public:
    void SetKeepLocal(bool val)
    {
        n_assert(!this->handled);
        this->keeplocal = val;
    };
    bool GetKeepLocal() const
    {
        return this->keeplocal;
    };
private:
    bool keeplocal;
public:
    void SetRotation(const InternalGraphics::AttachmentServer::AttachmentRotation& val)
    {
        n_assert(!this->handled);
        this->rotation = val;
    };
    const InternalGraphics::AttachmentServer::AttachmentRotation& GetRotation() const
    {
        return this->rotation;
    };
private:
    InternalGraphics::AttachmentServer::AttachmentRotation rotation;
public:
    void SetOffset(const Math::matrix44& val)
    {
        n_assert(!this->handled);
        this->offset = val;
    };
    const Math::matrix44& GetOffset() const
    {
        return this->offset;
    };
private:
    Math::matrix44 offset;
};
//------------------------------------------------------------------------------
class SwitchAttachmentToNewJoint : public BindAttachment
{
    __DeclareClass(SwitchAttachmentToNewJoint);
    __DeclareMsgId;
public:
    SwitchAttachmentToNewJoint() 
    { };
public:
    void SetNewJoint(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->newjoint = val;
    };
    const Util::StringAtom& GetNewJoint() const
    {
        return this->newjoint;
    };
private:
    Util::StringAtom newjoint;
};
//------------------------------------------------------------------------------
class BaseAnimEventMessage : public GraphicsEntityMessage
{
    __DeclareClass(BaseAnimEventMessage);
    __DeclareMsgId;
public:
    BaseAnimEventMessage() 
    { };
};
//------------------------------------------------------------------------------
class RegisterAnimEventHandler : public Messaging::Message
{
    __DeclareClass(RegisterAnimEventHandler);
    __DeclareMsgId;
public:
    RegisterAnimEventHandler() 
    { };
public:
    void SetAnimEventHandler(const Ptr<Animation::AnimEventHandlerBase>& val)
    {
        n_assert(!this->handled);
        this->animeventhandler = val;
    };
    const Ptr<Animation::AnimEventHandlerBase>& GetAnimEventHandler() const
    {
        return this->animeventhandler;
    };
private:
    Ptr<Animation::AnimEventHandlerBase> animeventhandler;
};
//------------------------------------------------------------------------------
class UnregisterAnimEventHandler : public Messaging::Message
{
    __DeclareClass(UnregisterAnimEventHandler);
    __DeclareMsgId;
public:
    UnregisterAnimEventHandler() 
    { };
public:
    void SetCategoryName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->categoryname = val;
    };
    const Util::StringAtom& GetCategoryName() const
    {
        return this->categoryname;
    };
private:
    Util::StringAtom categoryname;
};
//------------------------------------------------------------------------------
class PreloadMousePointerTextures : public Messaging::Message
{
    __DeclareClass(PreloadMousePointerTextures);
    __DeclareMsgId;
public:
    PreloadMousePointerTextures() 
    { };
public:
    void SetResourceIds(const Util::Array<Resources::ResourceId>& val)
    {
        n_assert(!this->handled);
        this->resourceids = val;
    };
    const Util::Array<Resources::ResourceId>& GetResourceIds() const
    {
        return this->resourceids;
    };
private:
    Util::Array<Resources::ResourceId> resourceids;
};
//------------------------------------------------------------------------------
class UpdateMousePointers : public Messaging::Message
{
    __DeclareClass(UpdateMousePointers);
    __DeclareMsgId;
public:
    UpdateMousePointers() 
    { };
public:
    void SetPointers(const Util::Array<CoreGraphics::MousePointer>& val)
    {
        n_assert(!this->handled);
        this->pointers = val;
    };
    const Util::Array<CoreGraphics::MousePointer>& GetPointers() const
    {
        return this->pointers;
    };
private:
    Util::Array<CoreGraphics::MousePointer> pointers;
};
//------------------------------------------------------------------------------
class HoldSharedResources : public Messaging::Message
{
    __DeclareClass(HoldSharedResources);
    __DeclareMsgId;
public:
    HoldSharedResources() 
    { };
};
//------------------------------------------------------------------------------
class ReleaseSharedResources : public Messaging::Message
{
    __DeclareClass(ReleaseSharedResources);
    __DeclareMsgId;
public:
    ReleaseSharedResources() 
    { };
};
//------------------------------------------------------------------------------
class SetAnimatorTime : public GraphicsEntityMessage
{
    __DeclareClass(SetAnimatorTime);
    __DeclareMsgId;
public:
    SetAnimatorTime() 
    { };
public:
    void SetNodeInstanceName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->nodeinstancename = val;
    };
    const Util::StringAtom& GetNodeInstanceName() const
    {
        return this->nodeinstancename;
    };
private:
    Util::StringAtom nodeinstancename;
public:
    void SetTime(const Timing::Time& val)
    {
        n_assert(!this->handled);
        this->time = val;
    };
    const Timing::Time& GetTime() const
    {
        return this->time;
    };
private:
    Timing::Time time;
};
//------------------------------------------------------------------------------
class UpdateCameraEntity : public GraphicsEntityMessage
{
    __DeclareClass(UpdateCameraEntity);
    __DeclareMsgId;
public:
    UpdateCameraEntity() 
    { };
public:
    void SetCameraSettings(const Shared::CameraSettings& val)
    {
        n_assert(!this->handled);
        this->camerasettings = val;
    };
    const Shared::CameraSettings& GetCameraSettings() const
    {
        return this->camerasettings;
    };
private:
    Shared::CameraSettings camerasettings;
};
//------------------------------------------------------------------------------
class SetShadowPointOfInterest : public Messaging::Message
{
    __DeclareClass(SetShadowPointOfInterest);
    __DeclareMsgId;
public:
    SetShadowPointOfInterest() 
    { };
public:
    void SetPoi(const Math::point& val)
    {
        n_assert(!this->handled);
        this->poi = val;
    };
    const Math::point& GetPoi() const
    {
        return this->poi;
    };
private:
    Math::point poi;
};
} // namespace Graphics
//------------------------------------------------------------------------------
