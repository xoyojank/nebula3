#pragma once
//------------------------------------------------------------------------------
/**
    @class Test::Drasa3TestUiApplication        

    (C) 2010 Radon Labs GmbH
*/
#include "apprender/viewerapplication.h"
#include "graphics/spotlightentity.h"
#include "graphics/modelentity.h"
#include "graphics/globallightentity.h"
#include "graphics/pointlightentity.h"
#include "ui/mainscreen.h"
#include "ui/uirenderer.h"
#include "ui/uirendermodule.h"
#include "ui/uimanager.h"

#include "GFxTutorial.h"
#include "GfxRendermodule.h"
#include "GfXRender.h"

//------------------------------------------------------------------------------
namespace Test
{
class Drasa3TestUiApplication : public App::ViewerApplication
{
public:
    /// constructor
    Drasa3TestUiApplication();
    /// destructor
    virtual ~Drasa3TestUiApplication();
    /// open the application
    virtual bool Open();
    /// close the application
    virtual void Close();

private:
    /// process input (called before rendering)
    virtual void OnProcessInput();
    /// render current frame
    virtual void OnUpdateFrame();
    /// called to configure display
    virtual void OnConfigureDisplay();

    /// update the GUI
    void UpdateGUI();

    Ptr<Graphics::ModelEntity> ground;
    Util::Array<Ptr<Graphics::ModelEntity> > models;
    Ptr<Graphics::GlobalLightEntity> globalLight;
    Util::Array<Ptr<Graphics::PointLightEntity> > pointLights;
    Ptr<Graphics::SpotLightEntity> testSpotLight;
    Util::Array<Ptr<Graphics::SpotLightEntity> > spotLights;
    Util::Array<Math::matrix44> lightTransforms; 
    Math::float4 shadowConstants;
    float direction;
    Util::Array<Timing::Time> frameTimes;
    Timing::Time avgFPS;
    bool benchmarkmode;
    bool renderDebug;
    float rotX;

    // client side stuff
    Ptr<UI::UIManager> uiManager;
    Ptr<UI::UIRenderer> uiRenderer;
    // connection to render thread
    Ptr<UI::UIRenderModule> uiRenderModule;

    Ptr<UI::MainScreen> mainWindow;

	Ptr<ScaleForms::GfxRenderModule> gfxRenderModule;
};

} // namespace Test
//------------------------------------------------------------------------------
