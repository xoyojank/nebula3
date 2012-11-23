//------------------------------------------------------------------------------
//  coreuirtplugin.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "ui/rt/coreuirtplugin.h"
#include "frame/framebatch.h"
#include "graphics/graphicsinterface.h"
#include "internalgraphics/internalview.h"

namespace CoreUI
{
__ImplementClass(CoreUI::CoreUIRTPlugin, 'CUTP', RenderModules::RTPlugin);

using namespace RenderModules;
using namespace Frame;
using namespace CoreGraphics;
using namespace Graphics;
using namespace InternalGraphics;

//------------------------------------------------------------------------------
/**
*/
CoreUIRTPlugin::CoreUIRTPlugin()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
CoreUIRTPlugin::~CoreUIRTPlugin()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
CoreUIRTPlugin::OnRegister()
{
    n_assert(!this->coreUiHandler.isvalid());
    n_assert(!this->coreUiRenderer.isvalid());

    // setup the render-thread UIrenderer
    this->coreUiRenderer = CoreUIRenderer::Create();
    this->coreUiRenderer->Setup();

    // setup handler
    this->coreUiHandler = CoreUIGraphicsHandler::Create();
    GraphicsInterface::Instance()->AttachHandler(this->coreUiHandler.upcast<Messaging::Handler>());
}

//------------------------------------------------------------------------------
/**
*/
void
CoreUIRTPlugin::OnUnregister()
{
    n_assert(this->coreUiHandler.isvalid());
    n_assert(this->coreUiRenderer.isvalid());

    GraphicsInterface::Instance()->RemoveHandler(this->coreUiHandler.upcast<Messaging::Handler>());
    this->coreUiHandler->Close();
    this->coreUiHandler = 0;                                                                           

    this->coreUiRenderer->Discard();
    this->coreUiRenderer = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
CoreUIRTPlugin::OnRenderWithoutView(IndexT frameId, Timing::Time time)
{
    const Ptr<InternalView>& view = InternalGraphicsServer::Instance()->GetDefaultView();
    if (!view.isvalid() || !view->GetCameraEntity().isvalid())
    {
        if (this->coreUiRenderer.isvalid()
            && this->coreUiRenderer->GetNumElements() > 0
            && this->coreUiRenderer->CheckTexturesLoaded())
        {
            RenderDevice* renderDevice = RenderDevice::Instance();
            if (renderDevice->BeginFrame())
            {
                const Ptr<CoreGraphics::ShaderInstance>& shdInst = this->coreUiRenderer->GetShader();
                renderDevice->BeginPass(renderDevice->GetDefaultRenderTarget(), shdInst);
                CoreUI::CoreUIRenderer::Instance()->DrawElements(shdInst);
                renderDevice->EndPass();
                renderDevice->EndFrame();
                renderDevice->Present();
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
CoreUIRTPlugin::OnRenderFrameBatch(const Ptr<FrameBatch>& frameBatch)
{    
    n_assert(this->coreUiRenderer.isvalid());

    if (frameBatch->GetType() == BatchType::UI)
    {
        this->coreUiRenderer->DrawElements(frameBatch->GetShader());
    }
}

} // namespace UI