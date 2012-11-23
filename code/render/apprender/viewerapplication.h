#pragma once
//------------------------------------------------------------------------------
/**
    @class App::ViewerApplication
    
    Derived from RenderApplication, adds support for Stages, Views
    and GraphicsEntities to the asynchronous render app.
    
    (C) 2007 Radon Labs GmbH
*/
#include "apprender/renderapplication.h"
#include "graphics/graphicsserver.h"
#include "graphics/stage.h"
#include "graphics/view.h"
#include "graphics/cameraentity.h"
#include "renderutil/mayacamerautil.h"
#include "debugrender/debugshaperenderer.h"
#include "debugrender/debugtextrenderer.h"

//------------------------------------------------------------------------------
namespace App
{
class ViewerApplication : public RenderApplication
{
public:
    /// constructor
    ViewerApplication();
    /// destructor
    virtual ~ViewerApplication();
    /// open the application
    virtual bool Open();
    /// close the application
    virtual void Close();

protected:    
    /// process input (called before rendering)
    virtual void OnProcessInput();
    /// update world 
    virtual void OnUpdateFrame();

    Ptr<Graphics::GraphicsServer> graphicsServer;
    Ptr<Debug::DebugShapeRenderer> debugShapeRenderer;
    Ptr<Debug::DebugTextRenderer> debugTextRenderer;
    Ptr<Graphics::Stage> stage;
    Ptr<Graphics::View> view;
    Ptr<Graphics::CameraEntity> camera;
    RenderUtil::MayaCameraUtil mayaCameraUtil;
};

} // namespace Graphics
//------------------------------------------------------------------------------

    