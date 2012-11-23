#pragma once
//------------------------------------------------------------------------------
/**
    @class UI::UIFeatureUnit
    
    The UIFeatureUnit delivers a Server and the needed Factory Stuff, to create
	and handle a complete UserInterface

	// TODO more desc

    (C) 2008 Radon Labs GmbH
*/
#include "game/featureunit.h"
#include "ui/uirendermodule.h"

//------------------------------------------------------------------------------
namespace UI
{
class UIEventHandler;
class UIFeatureUnit : public Game::FeatureUnit    
{
    __DeclareClass(UIFeatureUnit);
    __DeclareSingleton(UIFeatureUnit);   

public:
    /// constructor
    UIFeatureUnit();
    /// destructor
    virtual ~UIFeatureUnit();

    /// called from GameServer::ActivateProperties()
    virtual void OnActivate();
    /// called from GameServer::DeactivateProperties()
    virtual void OnDeactivate();
    
    /// called in the middle of the feature trigger cycle
    virtual void OnFrame();

	/// called when game debug visualization is on
	virtual void OnRenderDebug();

	/// sets the handler for the UI
    virtual void SetUIEventHandler(const Ptr<UI::UIEventHandler>& handler);

    /// set loading screen resource
    void SetLoadingScreenFilename(const Util::String& res);
    /// has loading resource id
    bool HasLoadingScreenReourceId() const;

protected:
    // connection to render thread
    Ptr<UI::UIRenderModule> uiRenderModule;

    Util::String loadingWindowFilename;
};

//------------------------------------------------------------------------------
/**
*/
inline void 
UIFeatureUnit::SetLoadingScreenFilename(const Util::String& res)
{
    this->loadingWindowFilename = res;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
UIFeatureUnit::HasLoadingScreenReourceId() const
{
    return this->loadingWindowFilename.IsValid();
}
}; // namespace UI
//------------------------------------------------------------------------------