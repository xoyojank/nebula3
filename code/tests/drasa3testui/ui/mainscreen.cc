//------------------------------------------------------------------------------
//  ui/mainscreen.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"

#include "ui/mainscreen.h"


#include "ui/event.h"
/*
#include "ui/securomwindow.h"
#include "application/dsaapp.h"
#include "application/dsamainscreenstate.h"
#include "application/dsalocationstate.h"
#include "managers/audio/dsamusicmanager.h"
#include "managers/dsasavegamemanager.h"
#include "managers/dsadebugmanager.h"
#include "managers/dsarules/statisticmanager.h"
#include "managers/audio/dsamusicmanager.h"
*/

namespace UI
{
__ImplementClass(MainScreen, 'UIMS', Window);

//using namespace Managers;
//using namespace Application;

//------------------------------------------------------------------------------
/**
*/
MainScreen::MainScreen() :
    inputDisabled(false)
{    
    this->SetResource("gui/mainscreen");
    this->SetScreenAlignmentX(Window::CenterX);
    this->SetScreenAlignmentY(Window::CenterY);
}

//------------------------------------------------------------------------------
/**
*/
MainScreen::~MainScreen()
{
    if (this->IsOpen())
    {
        this->Close();
    }    
}

//------------------------------------------------------------------------------
/**
*/
void
MainScreen::Open()
{   
    // reset input disabled flag (to prevent accidential input during fadeout)
    this->inputDisabled = false;
    Window::Open();        
    //DsaMusicManager::Instance()->CloseUIVolumeUp();
}

//------------------------------------------------------------------------------
/**
*/
void
MainScreen::Close()
{    
    Window::Close(); 
}

//------------------------------------------------------------------------------
/**
*/
void
MainScreen::HandleEvent(Event* e)
{
    //n_printf("MainScreen::HandleEvent()\n  eventNa me: %s\n  type: %d\n", e->GetEventName().AsCharPtr(), e->GetEventType());

    // if input disabled because we're currently fading out, do nothing
    if (this->inputDisabled)
    {
        return;
    }
 /*
    n_assert(e);
    App* app = App::Instance();    
    DsaMainScreenState* mainScreenState = (DsaMainScreenState*) app->FindStateHandlerByRtti(DsaMainScreenState::RTTI);

    const nString& eventName = e->GetEventName();
    bool playSound = false;
  
    if ("NewGame" == eventName)
    {   
        mainScreenState->ToggleWindows("DifficultyWindow");  
        playSound = true;
    }
    else if ("ContinueGame" == eventName)
    {        
		 
		///create a new session direktory and copy savegame statistics
		StatisticManager::Instance()->CreateSessionDirectory();

		///copy savegame statistics 
		Loader::UserProfile* userProfile = Loader::LoaderServer::Instance()->GetUserProfile();    
		nString saveGamePath = userProfile->GetSaveGamePath("continue_0") + "/";   

		nString statisticPath = StatisticManager::Instance()->GetSessionDirectoryPath() + "/"; 

		StatisticManager::Instance()->CopyStatisticFiles(saveGamePath,statisticPath);
		StatisticManager::Instance()->OnLoadedGameFromMainMenu(); 


		//handle continue game
        DsaLocationState* locationState = (DsaLocationState*) App::Instance()->FindStateHandlerByName("DsaLocation");
        n_assert(locationState->IsA(DsaLocationState::RTTI));
        locationState->SetSetupMode(GameStateHandler::ContinueGame);
        mainScreenState->FadeOutAndRequestState("DsaLocation");
        playSound = true;
    }
    else if ("LoadGame" == eventName)
    {       
        mainScreenState->ToggleWindows("LoadScreen");
        playSound = true;
    }
    else if ("Options" == eventName)
    {
#if (DSA_CHEATS)
    if (Managers::DsaDebugManager::Instance()->GetCheatsEnabled())
    {
        if (Input2::InputServer::Instance()->Keyboard()->KeyPressed(Input2::Key::Shift) && 
            Input2::InputServer::Instance()->Keyboard()->KeyPressed(Input2::Key::Control))
        {
            mainScreenState->ToggleDebugWindow();
            return;
        }
    }
#endif  
        mainScreenState->ToggleWindows("OptionsScreen");        
        playSound = true;
    }
    else if ("Credits" == eventName)
    {        
        mainScreenState->ToggleWindows("CreditsScreen");
        playSound = true;
    }
    else if ("Quit" == eventName)
    {        
        mainScreenState->FadeOutAndRequestState("Exit");
        playSound = true;
    }
    else if ("Extras" == eventName)
    {
        mainScreenState->ToggleWindows("Extras");
        playSound = true;
    }
    else
    {
        // start a demo location if corresponding button pressed,
        // for demo only so simply generate dictionary on the fly
        nDictionary<nString, nString> demoLocations = mainScreenState->SetupDemoLocations();
        int locationIndex = demoLocations.FindIndex(eventName);
        if (InvalidIndex != locationIndex)
        {           
            GameStateHandler* gameState = (GameStateHandler*) App::Instance()->FindStateHandlerByName("DsaLocation");
            n_assert(gameState->IsA(GameStateHandler::RTTI));
            gameState->SetSetupMode(GameStateHandler::NewGame);
            gameState->SetLevelName(demoLocations.ValueAtIndex(locationIndex));
            mainScreenState->FadeOutAndRequestState("DsaLocation");
            playSound = true;
        }
    }      
    if (playSound)
    {
        Audio::AudioServer::Instance()->Play2DSoundEffect("ui_click", 1.f);
    }
*/
}

}
