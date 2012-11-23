#pragma once
//------------------------------------------------------------------------------
/**
    @class Tests::TextureStreamingApplication
    
    Viewer for testing and demonstrating Nebula3 Texture Streaming.

    Press N at runtime to spawn up to 8 objects in a rectangle around the center.
    The demo pool has allocated memory for up to 3 objects so 3 objects can be displayed textured.
    Move or rotate camera so textured objects get out of camera view. As soon as any untextured object
    gets into camera view its textures will be loaded.

    If auto Mip Mapping is enabled textures may be replaced with lower mips and more than 3 objects will fit into memory.
    Currently the Resource Management System does NOT try to load a texture in a lower mip level if there is no
    memory for requested mip level. To change this edit TexturePoolMapperScheduler::OnRequestOtherMipMap().

    IMPORTANT: copy demopool-XMLs from work/textures to export_win32/textures before starting the demo!

    democase 0 - 3 of 8 tanks can be loaded
    democase 1 - ground can be loaded in 3 different mip stages

    (C) 2010 Radon Labs GmbH
*/
#include "apprender/viewerapplication.h"
#include "graphics/spotlightentity.h"
#include "graphics/modelentity.h"
#include "graphics/globallightentity.h"
#include "graphics/pointlightentity.h"
#include "render/resources/streaming/poolresourcemapper.h"

//------------------------------------------------------------------------------
namespace Tests
{
class TextureStreamingApplication : public App::ViewerApplication
{
public:
    /// constructor
    TextureStreamingApplication();
    /// destructor
    virtual ~TextureStreamingApplication();
    /// open the application
    virtual bool Open();
    /// close the application
    virtual void Close();

protected:
    /// called to setup resource mappers
    virtual void OnSetupResourceMappers();
    
private:
    /// process input (called before rendering)
    virtual void OnProcessInput();
    /// render current frame
    virtual void OnUpdateFrame();
    /// called to configure display
    virtual void OnConfigureDisplay();
    /// append a test model depending on current test case
    virtual void AppendTestModel();

    int modelCounter;
    int numModels;
    int numModelsCreated;

    Ptr<Graphics::ModelEntity> ground;
    Util::Array<Ptr<Graphics::ModelEntity> > models;
    Ptr<Graphics::GlobalLightEntity> globalLight;
    Util::Array<Ptr<Graphics::PointLightEntity> > pointLights;
    Ptr<Graphics::SpotLightEntity> testSpotLight;
    Util::Array<Ptr<Graphics::SpotLightEntity> > spotLights;
    Util::Array<Math::matrix44> lightTransforms; 
    Ptr<Graphics::PointLightEntity> testPointLight;
    Math::float4 shadowConstants;
    float direction;
    Util::Array<Timing::Time> frameTimes;
    Timing::Time avgFPS;
    bool benchmarkmode;
    bool renderDebug;
    float rotX;
    bool capturing;

    int demoCase;
};

} // namespace Test
//------------------------------------------------------------------------------
