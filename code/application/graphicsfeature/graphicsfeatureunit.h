#pragma once
//------------------------------------------------------------------------------
/**
    @class GraphicsFeature::GraphicsFeatureUnitUnit
    
    The GraphicsFeatures provides everything for rendering graphicentities
    from different views in different stages.

    For the default use it creates one default view and one default stage which are
    used in the graphic and camera properties.    
    The BaseGameFeatureUnit uses this features to build up a default graphic world.

    Additonal to the rendering the graphicsfeature creates and triggers 
    the input server.

    (C) 2007 Radon Labs GmbH
*/
#include "game/featureunit.h"
#include "graphics/graphicsinterface.h"
#include "graphics/graphicsserver.h"
#include "graphics/display.h"
#include "graphics/stage.h"
#include "graphics/view.h"
#include "graphics/cameraentity.h"
#include "input/inputserver.h"
#include "debugrender/debugshaperenderer.h"
#include "debugrender/debugtextrenderer.h"
#include "properties/pathanimproperty.h"
#include "posteffect/posteffectmanager.h"
#include "graphicsfeature/managers/attachmentmanager.h"
#include "debug/debugtimer.h"
#include "framesync/framesynctimer.h"
#include "resources/resourcemanager.h"
#if !(__WII__ || __PS3__)
#include "addons/posteffect/posteffectmanager.h"
#endif

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class GraphicsFeatureUnit : public Game::FeatureUnit    
{
    __DeclareClass(GraphicsFeatureUnit);
    __DeclareSingleton(GraphicsFeatureUnit);   
public:
    /// constructor
    GraphicsFeatureUnit();
    /// destructor
    virtual ~GraphicsFeatureUnit();

    /// called from GameServer::ActivateProperties()
    virtual void OnActivate();
    /// called from GameServer::DeactivateProperties()
    virtual void OnDeactivate();           
    /// called from within GameServer::OnStart() after OnLoad when the complete world exist
    virtual void OnStart();      
    /// called on begin of frame
    virtual void OnBeginFrame();
    /// called in the middle of the feature trigger cycle
    virtual void OnFrame();        
    /// called at the end of the feature trigger cycle
    virtual void OnEndFrame();
    /// start render debug
    virtual void StartRenderDebug();
    /// stop render debug
    virtual void StopRenderDebug();

    /// create default stage, view and camera
    void SetupDefaultGraphicsWorld();  
    /// discard default graphics world
    void DiscardDefaultGraphicsWorld();
    /// get the default stage
    const Ptr<Graphics::Stage>& GetDefaultStage() const;
    /// get the default view
    const Ptr<Graphics::View>& GetDefaultView() const;
    /// get default camera entity
    const Ptr<Graphics::CameraEntity>& GetDefaultCamera() const;
    /// on entities loaded
    void OnEntitiesLoaded();       
    /// on setup resource mappers
    void OnSetupResourceMappers();

protected:
    /// called to configure display device
    virtual void OnConfigureDisplay();

    Ptr<Graphics::GraphicsInterface> graphicsInterface;
    Ptr<Graphics::Display> display;
    Ptr<FrameSync::FrameSyncTimer> frameSyncTimer;
    Ptr<Graphics::GraphicsServer> graphicsServer;
    Ptr<Graphics::CameraEntity> defaultCamera;
    Ptr<Input::InputServer> inputServer;
    Ptr<Graphics::Stage> defaultStage;
    Ptr<Graphics::View> defaultView;
    Ptr<Debug::DebugShapeRenderer> debugShapeRenderer;
    Ptr<Debug::DebugTextRenderer> debugTextRenderer;
    Ptr<Resources::ResourceManager> resManager;
    #if !(__WII__)
    Ptr<PostEffect::PostEffectManager> postEffectManager;
    #endif
    Ptr<GraphicsFeature::AttachmentManager> attachmentManager;

    _declare_timer(WaitForGraphicsThreadTime)
};

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Graphics::View>&
GraphicsFeatureUnit::GetDefaultView() const
{
    return this->defaultView;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Graphics::Stage>&
GraphicsFeatureUnit::GetDefaultStage() const
{
    return this->defaultStage;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Graphics::CameraEntity>&
GraphicsFeatureUnit::GetDefaultCamera() const
{
    return this->defaultCamera;
}

} // namespace GraphicsFeatureUnit
//------------------------------------------------------------------------------
