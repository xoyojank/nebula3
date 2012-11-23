//------------------------------------------------------------------------------
//  internalgraphicshandler.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/graphicsprotocol.h"
#include "messaging/staticmessagehandler.h"
#include "coregraphics/displaydevice.h"
#include "coregraphics/renderdevice.h"
#include "coregraphics/shaderserver.h"
#include "internalgraphics/internalgraphicsserver.h"
#include "internalgraphics/internalview.h"
#include "frame/frameserver.h"
#include "animation/animeventserver.h"
#include "coregraphics/mouserenderdevice.h"
#include "framesync/framesynctimer.h"
#include "lighting/shadowserver.h"
#include "resources/resourcemanager.h"
#include "particles/particleserver.h"

using namespace Util;
using namespace CoreGraphics;
using namespace Graphics;
using namespace InternalGraphics;
using namespace Resources;
using namespace Frame;
using namespace Lighting;

namespace Messaging
{

//------------------------------------------------------------------------------
/**
*/
__StaticHandler(AdapterExists)
{
    msg->SetResult(DisplayDevice::Instance()->AdapterExists(msg->GetAdapter()));
}

//------------------------------------------------------------------------------
/**
*/
__StaticHandler(GetAvailableDisplayModes)
{
    Adapter::Code adapter = msg->GetAdapter();
    PixelFormat::Code pixelFormat = msg->GetPixelFormat();
    msg->SetResult(DisplayDevice::Instance()->GetAvailableDisplayModes(adapter, pixelFormat));
}

//------------------------------------------------------------------------------
/**
*/
__StaticHandler(AdjustDisplaySize)
{
	DisplayDevice::Instance()->AdjustSize();
}

//------------------------------------------------------------------------------
__StaticHandler(GetCurrentAdapterDisplayMode)
{
    Adapter::Code adapter = msg->GetAdapter();
    msg->SetResult(DisplayDevice::Instance()->GetCurrentAdapterDisplayMode(adapter));
}

//------------------------------------------------------------------------------
/**
*/
__StaticHandler(GetAdapterInfo)
{
    Adapter::Code adapter = msg->GetAdapter();
    msg->SetResult(DisplayDevice::Instance()->GetAdapterInfo(adapter));
}

//------------------------------------------------------------------------------
/**
*/
__StaticHandler(AttachDisplayEventHandler)
{
    DisplayDevice::Instance()->AttachEventHandler(msg->GetHandler().upcast<DisplayEventHandler>());
}

//------------------------------------------------------------------------------
/**
*/
__StaticHandler(RemoveDisplayEventHandler)
{
    DisplayDevice::Instance()->RemoveEventHandler(msg->GetHandler().upcast<DisplayEventHandler>());
}

//------------------------------------------------------------------------------
/**
*/
__StaticHandler(AttachRenderEventHandler)
{
    RenderDevice::Instance()->AttachEventHandler(msg->GetHandler().upcast<RenderEventHandler>());
}

//------------------------------------------------------------------------------
/**
*/
__StaticHandler(RemoveRenderEventHandler)
{
    RenderDevice::Instance()->RemoveEventHandler(msg->GetHandler().upcast<RenderEventHandler>());
}

//------------------------------------------------------------------------------
/**
*/
__StaticHandler(CreateGraphicsStage)
{
    const StringAtom& name = msg->GetName();
    Ptr<InternalStage> stage = InternalGraphicsServer::Instance()->CreateStage(name, msg->GetVisibleSystems());
    msg->GetObjectRef()->Validate<InternalStage>(stage.get());
}

//------------------------------------------------------------------------------
/**
*/
__StaticHandler(DiscardGraphicsStage)
{
    Ptr<InternalStage> stage = msg->GetObjectRef()->Ref<InternalStage>();
    InternalGraphicsServer::Instance()->DiscardStage(stage);

    // important to invalidate object ref, so that object not released in the main thread!
    msg->GetObjectRef()->Invalidate();
}

//------------------------------------------------------------------------------
/**
*/
__StaticHandler(CreateGraphicsView)
{
    const Core::Rtti* viewClass = msg->GetViewClass();
    n_assert(viewClass && viewClass->IsDerivedFrom(InternalView::RTTI));
    const StringAtom& viewName = msg->GetName();
    const StringAtom& stageName = msg->GetStageName();
    const ResourceId& frameShaderName = msg->GetFrameShaderName();
    bool isDefaultView = msg->GetDefaultView();

    Ptr<FrameShader> frameShader = FrameServer::Instance()->LookupFrameShader(frameShaderName);
    const Ptr<InternalStage>& stage = InternalGraphicsServer::Instance()->GetStageByName(stageName);
    Ptr<InternalView> view = InternalGraphicsServer::Instance()->CreateView(*viewClass, viewName, isDefaultView);
    view->SetStage(stage);
    view->SetFrameShader(frameShader);

    msg->GetObjectRef()->Validate<InternalView>(view.get());
}

//------------------------------------------------------------------------------
/**
*/
__StaticHandler(DiscardGraphicsView)
{
    Ptr<InternalView> view = msg->GetObjectRef()->Ref<InternalView>();
    InternalGraphicsServer::Instance()->DiscardView(view);

    // important to invalidate object ref, so that object not released in the main thread!
    msg->GetObjectRef()->Invalidate();
}

//------------------------------------------------------------------------------
/**
*/
__StaticHandler(SetDefaultGraphicsView)
{
    Ptr<InternalView> view = msg->GetObjectRef()->Ref<InternalView>();
    InternalGraphicsServer::Instance()->SetDefaultView(view);
}

//------------------------------------------------------------------------------
/**
*/
__StaticHandler(RegisterAnimEventHandler)
{
    Animation::AnimEventServer::Instance()->RegisterAnimEventHandler(msg->GetAnimEventHandler());
}

//------------------------------------------------------------------------------
/**
*/
__StaticHandler(UnregisterAnimEventHandler)
{
    Animation::AnimEventServer::Instance()->UnregisterAnimEventHandler(msg->GetCategoryName());  
}

//------------------------------------------------------------------------------
/**
*/
__StaticHandler(PreloadMousePointerTextures)
{
    MouseRenderDevice::Instance()->PreloadTextures(msg->GetResourceIds());
}

//------------------------------------------------------------------------------
/**
*/
__StaticHandler(UpdateMousePointers)
{
    MouseRenderDevice::Instance()->UpdatePointers(msg->GetPointers());
}

//------------------------------------------------------------------------------
/**
*/
__StaticHandler(RegisterRTPlugin)
{
    RenderModules::RTPluginRegistry::Instance()->RegisterRTPlugin(msg->GetType());
}

//------------------------------------------------------------------------------
/**
*/
__StaticHandler(UnregisterRTPlugin)
{
    RenderModules::RTPluginRegistry::Instance()->UnregisterRTPlugin(msg->GetType());
}

//------------------------------------------------------------------------------
/**
*/
__StaticHandler(SetTimeFactor)
{
    FrameSync::FrameSyncTimer::Instance()->SetTimeFactor(msg->GetFactor());

    // @todo: what's this??
#if !(__PS3__)
    Particles::ParticleSystem::ModulateStepTime(msg->GetFactor());
#endif
}

//------------------------------------------------------------------------------
/**
*/
__StaticHandler(UpdateSharedShaderVariable)
{
    const Ptr<ShaderVariable>& shaderVar = ShaderServer::Instance()->GetSharedVariableBySemantic(msg->GetSemantic());
    switch (msg->GetValue().GetType())
    {
        case Variant::Float:
            shaderVar->SetFloat(msg->GetValue().GetFloat());
            break;
        case Variant::Float4:
            shaderVar->SetFloat4(msg->GetValue().GetFloat4());
            break;
        case Variant::Matrix44:
            shaderVar->SetMatrix(msg->GetValue().GetMatrix44());
            break;   
        case Variant::FloatArray:
            shaderVar->SetFloatArray(msg->GetValue().GetFloatArray().Begin(), msg->GetValue().GetFloatArray().Size());
            break;
        case Variant::Float4Array: 
            shaderVar->SetFloat4Array(msg->GetValue().GetFloat4Array().Begin(), msg->GetValue().GetFloat4Array().Size());
            break;
        case Variant::Matrix44Array:
            shaderVar->SetMatrixArray(msg->GetValue().GetMatrix44Array().Begin(), msg->GetValue().GetMatrix44Array().Size());
            break;

        case Variant::Void:
        case Variant::Int:
        case Variant::Bool:        
        case Variant::String:        
        case Variant::Blob:
        case Variant::Guid:
        case Variant::Object:
        case Variant::IntArray:
        case Variant::BoolArray:
        case Variant::BlobArray:
        case Variant::StringArray:
        case Variant::GuidArray:
            n_error("OnUpdateSharedShaderVariable: type not supported!");
            break;                                                    
    }
}

//------------------------------------------------------------------------------
/**
*/
__StaticHandler(SetShadowPointOfInterest)
{
    ShadowServer::Instance()->SetPointOfInterest(msg->GetPoi());
}

//------------------------------------------------------------------------------
/**
*/
__StaticHandler(HoldSharedResources)
{
    // @todo: remove this message and related stuff as new resource management won't need this (at least it shouldn't ...)
    ResourceManager::Instance()->HoldResources();
}

//------------------------------------------------------------------------------
/**
*/
__StaticHandler(ReleaseSharedResources)
{
    // @todo: remove this message and related stuff as new resource management won't need this (at least it shouldn't ...)
    ResourceManager::Instance()->ReleaseResources();
}

} // namespace Messaging

