//------------------------------------------------------------------------------
//  FxFeature/FxFeatureUnit.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "fx/fxfeature/fxfeatureunit.h"

namespace FxFeature
{
__ImplementClass(FxFeature::FxFeatureUnit, 'FXFU' , Game::FeatureUnit);
__ImplementSingleton(FxFeature::FxFeatureUnit);

//------------------------------------------------------------------------------
/**
*/
FxFeatureUnit::FxFeatureUnit()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
FxFeatureUnit::~FxFeatureUnit()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
FxFeatureUnit::OnActivate()
{
    FeatureUnit::OnActivate();

    // setup anim event registry if not yet done
    this->animEventRegistry = FX::AnimEventRegistry::Create();
    this->animEventRegistry->Setup();

    // create fxmanager and enable camera shake mixer
    this->fxManager = FxManager::Create();
    this->fxManager->Open();
    this->fxManager->SetupCameraShakeMixer(Math::vector(1, 1, 1), Math::vector(1, 1, 1));
}

//------------------------------------------------------------------------------
/**
*/
void
FxFeatureUnit::OnDeactivate()
{
    this->fxManager->Close();
    this->fxManager = 0;

    this->animEventRegistry->Discard();
    this->animEventRegistry = 0;

    FeatureUnit::OnDeactivate();    
}

//------------------------------------------------------------------------------
/**
*/
void
FxFeatureUnit::OnLoad()
{
    //empty
}

//------------------------------------------------------------------------------
/**
*/
void
FxFeatureUnit::OnBeginFrame()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
FxFeatureUnit::OnEndFrame()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
FxFeatureUnit::OnFrame()
{
    // empty
}
};