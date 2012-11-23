#pragma once
#ifndef TOOLS_TESTVIEWER_H
#define TOOLS_TESTVIEWER_H
//------------------------------------------------------------------------------
/**
    @class Tools::TestViewer
    
    Nebula3 test viewer app.

    (C) 2006 Radon Labs GmbH
*/
#include "apprender/viewerapplication.h"
#include "graphics/spotlightentity.h"
#include "graphics/modelentity.h"
#include "graphics/globallightentity.h"

//------------------------------------------------------------------------------
namespace Tools
{
class TestRemoteViewerApplication : public App::ViewerApplication
{
public:
    /// constructor
    TestRemoteViewerApplication();
    /// destructor
    virtual ~TestRemoteViewerApplication();
    /// open the application
    virtual bool Open();
    /// close the application
    virtual void Close();

    const Ptr<Graphics::ModelEntity>& GetModel() const;
private:
    /// render current frame
    virtual void OnUpdateFrame();

    Ptr<Graphics::ModelEntity> ground;
    Ptr<Graphics::ModelEntity> model;
    Ptr<Graphics::GlobalLightEntity> globalLight;
    Ptr<Graphics::SpotLightEntity> localLight0;
};
//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Graphics::ModelEntity>& TestRemoteViewerApplication::GetModel() const
{
    return model;
}

} // namespace Test
//------------------------------------------------------------------------------
#endif
