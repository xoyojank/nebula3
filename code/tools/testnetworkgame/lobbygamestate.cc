//------------------------------------------------------------------------------
//  testnetworkgame/testgamestate.cc
//  (C) 2009 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "testnetworkgame/lobbygamestate.h"
#include "basegamefeature/managers/entitymanager.h"
#include "debugrender/debugrender.h"
#include "input/inputserver.h"
#include "input/keyboard.h"
#include "input/gamepad.h"
#include "input/key.h"
#include "multiplayer/multiplayermanager.h"
#include "internalmultiplayer/multiplayertype.h"
#include "internalmultiplayer/internalmultiplayerinterface.h"
#include "testnetworkgameapplication.h"
#include "graphicsfeature/graphicsfeatureunit.h"
#include "graphics/cameraentity.h"

namespace Tools
{
__ImplementClass(Tools::LobbyGameState, 'LOGS', BaseGameFeature::GameStateHandler);

using namespace Util;
using namespace BaseGameFeature;
using namespace GraphicsFeature;
using namespace Input;
using namespace Multiplayer;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
LobbyGameState::LobbyGameState() : 
    startingHosting(false),
    autoHost(false),
    autoClient(false)
{
    this->btnColors.Add(Util::StringAtom("A"), point(0,1,0));
    this->btnColors.Add(Util::StringAtom("B"), point(1,0,0));
    this->btnColors.Add(Util::StringAtom("X"), point(0,0,1));
    this->btnColors.Add(Util::StringAtom("Y"), point(1,1,0));
    this->btnColors.Add(Util::StringAtom("RB"), point(0,1,1));
}

//------------------------------------------------------------------------------
/**
*/
LobbyGameState::~LobbyGameState()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
LobbyGameState::OnStateEnter(const Util::String& prevState)
{       
    BaseGameFeature::GameStateHandler::OnStateEnter(prevState);

    // set triggerstuff
    BaseGameFeature::EntityManager::Instance()->SetMaxTriggerDistance(9999999999);

    // get current camera and set nicer view position
    const Ptr<Graphics::CameraEntity>& cam = GraphicsFeature::GraphicsFeatureUnit::Instance()->GetDefaultCamera();
    matrix44 trans = matrix44::transformation(point::origin(), 
                                              quaternion::identity(), 
                                              vector(1,1,1), 
                                              point::origin(), 
                                              quaternion::rotationyawpitchroll(n_deg2rad(45),n_deg2rad(-15),0), 
                                              point(1,5,0));
    cam->SetTransform(trans);
}

//------------------------------------------------------------------------------
/**
*/
void
LobbyGameState::OnStateLeave(const Util::String& nextState)
{
    //BaseGameFeatureUnit::Instance()->RemoveManager(this->animationManager.cast<Game::Manager>());
    //this->animationManager = 0;

    GameStateHandler::OnStateLeave(nextState);
}

//------------------------------------------------------------------------------
/**
*/
Util::String 
LobbyGameState::OnFrame()
{
    this->numMenuEntries = 0;
    MultiplayerManager::GameFunction multiplayerFunction = MultiplayerManager::Instance()->GetGameFunction();
    MultiplayerManager::GameState multiplayerState = MultiplayerManager::Instance()->GetGameState();
    MultiplayerManager::SessionState sessionState = MultiplayerManager::Instance()->GetSessionState();

    // render current multiplayer state
    this->RenderMenuEntry("CurrentFunction:", MultiplayerManager::GameFunctionAsString(multiplayerFunction));
    this->RenderMenuEntry("CurrentState:", MultiplayerManager::GameStateAsString(multiplayerState));
    this->RenderMenuEntry("","");

    if (multiplayerState == MultiplayerManager::Idle
        || multiplayerState == MultiplayerManager::Running)
    {
        switch (multiplayerFunction)
        {
        case MultiplayerManager::None:
            if (MultiplayerManager::Instance()->GetNumLocalPlayers() == 0)
            {
                this->HandleLogin(); 
                
                if (this->autoHost || this->autoClient)
                {       
                    if (!Multiplayer::MultiplayerManager::Instance()->IsCreatePlayersActive())
                    {
                        Multiplayer::MultiplayerManager::Instance()->CreatePlayers(1);
                    }                             
                }
            }
            else
            {                
                MultiplayerManager::GameSearchState searchState = MultiplayerManager::Instance()->GetGameSearchState();
                if (searchState == MultiplayerManager::GameSearchNoResults)
                {
                    this->HandleIdle();
                
                    if (this->autoClient)
                    {    
                        Ptr<Base::ParameterResolverBase> paraResBase = Base::ParameterResolverBase::Create();                        
                        Multiplayer::MultiplayerManager::Instance()->StartGameSearch(paraResBase, 2, 10); 
                    }
                }
                else
                {
                    this->HandleGameSearch();
                }
                if (this->autoHost)
                {       
                    Ptr<Base::ParameterResolverBase> paraResBase = Base::ParameterResolverBase::Create();  
                    Multiplayer::MultiplayerManager::Instance()->HostSession(paraResBase, 2, 2, InternalMultiplayer::MultiplayerType::OnlineStandard, 0);       
                    MultiplayerFeature::DistributionManager::Instance()->RequestMode(MultiplayerFeature::DistributionManager::Host);
                }
            }

            
            break;
        case MultiplayerManager::InitializingAsHost:        
            break;
        case MultiplayerManager::HostingGame:

            if (sessionState == MultiplayerManager::SessionCreated)
            {                 
                if (multiplayerState == MultiplayerManager::Idle)
                {
                    this->HandleHostingSession();
                }
                else if (multiplayerState == MultiplayerManager::Running)
                {
                    this->HandleHostRunningGame(); 
                }      
            }                 
            break;
        case MultiplayerManager::InitializingAsClient:        
            break;
        case MultiplayerManager::JoinedGame:
            if (multiplayerState == MultiplayerManager::Idle)
            {
                this->HandleClientGameLobby();
            }
            else if (multiplayerState == MultiplayerManager::Running)
            {
                this->HandleClientRunningGame();
            }
            break;
        }
    }

    return GameStateHandler::OnFrame();
}

//------------------------------------------------------------------------------
/**
*/
void 
LobbyGameState::HandleLogin()
{    
    this->RenderMenuEntry("A", " - Login");

    Ptr<Keyboard> keyboard = InputServer::Instance()->GetDefaultKeyboard();
    Ptr<GamePad> pad = InputServer::Instance()->GetDefaultGamePad(0);

    if (this->IsBtnAction("A"))
    {
        if (!Multiplayer::MultiplayerManager::Instance()->IsCreatePlayersActive())
        {
    #if __WIN32__        
            Multiplayer::MultiplayerManager::Instance()->CreatePlayers(1);
    #else
            Multiplayer::MultiplayerManager::Instance()->CreatePlayers(2);
    #endif
        }
    } 
}

//------------------------------------------------------------------------------
/**
*/
void 
LobbyGameState::HandleIdle()
{
    // render debug stuff
   this->RenderMenuEntry("X", " - Host Game");
   this->RenderMenuEntry("A", " - Start game search");

    Ptr<Keyboard> keyboard = InputServer::Instance()->GetDefaultKeyboard();
    Ptr<GamePad> pad = InputServer::Instance()->GetDefaultGamePad(0);

    if (this->IsBtnAction("X"))
    {
        Ptr<Base::ParameterResolverBase> paraResBase = Base::ParameterResolverBase::Create();  
        Multiplayer::MultiplayerManager::Instance()->HostSession(paraResBase, 2, 2, InternalMultiplayer::MultiplayerType::OnlineStandard, 0);
        MultiplayerFeature::DistributionManager::Instance()->RequestMode(MultiplayerFeature::DistributionManager::Host);
    }
    else if (this->IsBtnAction("A"))
    {
        // start a game search
        Ptr<Base::ParameterResolverBase> paraResBase = Base::ParameterResolverBase::Create();  
        Multiplayer::MultiplayerManager::Instance()->StartGameSearch(paraResBase, 2, 10);        
    }    
}

//------------------------------------------------------------------------------
/**
*/
void 
LobbyGameState::HandleHostingSession()
{
    // only show if one other player has joined and set its player state to ready    
    const Ptr<Session>& session = MultiplayerManager::Instance()->GetSession();
    // list all session players
    const Util::Array< Ptr<Player> >& publicPlayers = session->GetPublicPlayers();
    IndexT i;
    for (i = 0; i < publicPlayers.Size(); ++i)
    {        
        Util::String isReadyString = publicPlayers[i]->IsReady() ? "ready" : "not ready";
        this->RenderMenuEntry("PublicSlot: ", publicPlayers[i]->GetGamerTag().AsString() + " is " + isReadyString);
    }
    const Util::Array< Ptr<Player> >& privatePlayers = session->GetPrivatePlayers();
    for (i = 0; i < privatePlayers.Size(); ++i)
    {
        Util::String isReadyString = privatePlayers[i]->IsReady() ? "ready" : "not ready";
        this->RenderMenuEntry("PrivateSlot: ", privatePlayers[i]->GetGamerTag().AsString() + " is " + isReadyString);
    }

    // empty line
    this->RenderMenuEntry("", "");
    if (session->GetNumRemotePlayers() > 0)
    {
        if  (session->AreAllPlayersReady())
        {
            this->RenderMenuEntry("Y", " - Start game");
            if (this->IsBtnAction("Y"))
            {
                // start a game 
                Multiplayer::MultiplayerManager::Instance()->StartGame();
                // load level
                Tools::TestNetworkGameApplication::Instance()->RequestState("GameState");
            }
        }
        else
        {
            this->RenderMenuEntry("", "Waiting for all Players get ready!");            
        }

        if (this->autoHost)
        {

            // start a game 
            Multiplayer::MultiplayerManager::Instance()->StartGame();   

            // load level
            Tools::TestNetworkGameApplication::Instance()->RequestState("GameState");
        }
    }
    else
    {
        this->RenderMenuEntry("", "Waiting for Players to join!");
    }

    this->RenderMenuEntry("A", " - Toggle Player Ready");
    if (this->IsBtnAction("A"))
    {
        // toggle player ready
        bool isReady = Multiplayer::MultiplayerManager::Instance()->GetPlayerReady(0);
        Multiplayer::MultiplayerManager::Instance()->SetPlayerReady(!isReady, 0);
    }
    this->RenderMenuEntry("B", " - End Session");
    if (this->IsBtnAction("B"))
    {
        Multiplayer::MultiplayerManager::Instance()->DeleteSession();
        MultiplayerFeature::DistributionManager::Instance()->RequestMode(MultiplayerFeature::DistributionManager::Idle);
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
LobbyGameState::HandleGameSearch()
{    
    // if gamesearch has finished
    MultiplayerManager::GameSearchState searchState = MultiplayerManager::Instance()->GetGameSearchState();
    if (Multiplayer::MultiplayerManager::GameSearchFinished == searchState)
    {
        const Util::Array<InternalMultiplayer::SessionInfo>& sessionArray = Multiplayer::MultiplayerManager::Instance()->GetLastGameSearchResults();
        if (sessionArray.Size() > 0)
        {
            Util::String ipAddress = sessionArray[0].GetIpAddressaAsString();            
            this->RenderMenuEntry("Y", " - Join Session at " + ipAddress);

            if (this->IsBtnAction("Y")|| this->autoClient)
            {
                Multiplayer::MultiplayerManager::Instance()->JoinSession(sessionArray[0], 0);
                MultiplayerFeature::DistributionManager::Instance()->RequestMode(MultiplayerFeature::DistributionManager::Client);
            }
        }
        else
        {
            this->RenderMenuEntry(" ", "No gamesessions found!");
            this->RenderMenuEntry("A", " - Start GameSearch again");

            if (this->IsBtnAction("A")|| this->autoClient)
            {
                Multiplayer::MultiplayerManager::Instance()->ClearLastGameSearch();
                // start a game search
                Ptr<Base::ParameterResolverBase> para = Base::ParameterResolverBase::Create();  
                Multiplayer::MultiplayerManager::Instance()->StartGameSearch(para, 2, 10);
            }
        }            

        this->RenderMenuEntry("B", " - Go back");
        if (this->IsBtnAction("B"))
        {
            Multiplayer::MultiplayerManager::Instance()->ClearLastGameSearch();    
            MultiplayerFeature::DistributionManager::Instance()->RequestMode(MultiplayerFeature::DistributionManager::Idle);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
LobbyGameState::HandleClientGameLobby()
{
    // show all players in lobby
    const Ptr<Session>& session = MultiplayerManager::Instance()->GetSession();
    // list all session players
    const Util::Array< Ptr<Player> >& publicPlayers = session->GetPublicPlayers();
    IndexT i;
    for (i = 0; i < publicPlayers.Size(); ++i)
    {        
        Util::String isReadyString = publicPlayers[i]->IsReady() ? "ready" : "not ready";
        this->RenderMenuEntry("PublicSlot: ", publicPlayers[i]->GetGamerTag().AsString() + " is " + isReadyString);
    }
    const Util::Array< Ptr<Player> >& privatePlayers = session->GetPrivatePlayers();
    for (i = 0; i < privatePlayers.Size(); ++i)
    {
        Util::String isReadyString = privatePlayers[i]->IsReady() ? "ready" : "not ready";
        this->RenderMenuEntry("PrivateSlot: ", privatePlayers[i]->GetGamerTag().AsString() + " is " + isReadyString);
    }

    this->RenderMenuEntry("A", " - Toggle Player Ready");
    if (this->IsBtnAction("A"))
    {
        // toggle player ready
        bool isReady = Multiplayer::MultiplayerManager::Instance()->GetPlayerReady(0);
        Multiplayer::MultiplayerManager::Instance()->SetPlayerReady(!isReady, 0);
    }
    this->RenderMenuEntry("B", " - Leave Session");
    if (this->IsBtnAction("B"))
    {
        Multiplayer::MultiplayerManager::Instance()->LeaveSession(0);
        MultiplayerFeature::DistributionManager::Instance()->RequestMode(MultiplayerFeature::DistributionManager::Idle);
    }   
}

//------------------------------------------------------------------------------
/**
*/
void 
LobbyGameState::HandleClientRunningGame()
{
    // show all players in lobby
    const Ptr<Session>& session = MultiplayerManager::Instance()->GetSession();
    // list all session players
    const Util::Array< Ptr<Player> >& publicPlayers = session->GetPublicPlayers();
    IndexT i;
    for (i = 0; i < publicPlayers.Size(); ++i)
    {        
        this->RenderMenuEntry("PublicSlot: ", publicPlayers[i]->GetGamerTag().Value());
    }
    const Util::Array< Ptr<Player> >& privatePlayers = session->GetPrivatePlayers();
    for (i = 0; i < privatePlayers.Size(); ++i)
    {
        this->RenderMenuEntry("PrivateSlot: ", privatePlayers[i]->GetGamerTag().Value());
    }
    this->RenderMenuEntry("B", " - End Game");
    if (this->IsBtnAction("B"))
    {
        Multiplayer::MultiplayerManager::Instance()->EndGame();
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
LobbyGameState::RenderMenuEntry(const Util::String& button, const Util::String& desc)
{    
    const float2 menuPosition(0.05f, 0.05f);

    point color(1,1,1);
    if (this->btnColors.Contains(button))
    {
        color = this->btnColors[button];
    }
    float2 entryOffset(0.0, this->numMenuEntries * 0.03f);
    _debug_text(button, menuPosition + entryOffset, color);

    SizeT strLen = button.Length();
    _debug_text(desc, menuPosition + float2(strLen * 0.008f, 0) + entryOffset, Math::float4(1,1,1,1));
    this->numMenuEntries++;
}

//------------------------------------------------------------------------------
/**
*/
bool 
LobbyGameState::IsBtnAction(const Util::String& button)
{
    Ptr<Keyboard> keyboard = InputServer::Instance()->GetDefaultKeyboard();
    Ptr<GamePad> pad = InputServer::Instance()->GetDefaultGamePad(0);
    if (button == "A")
    {
        return (pad->ButtonDown(GamePad::AButton) || keyboard->KeyDown(Key::A));
    }
    else if (button == "B")
    {
        return (pad->ButtonDown(GamePad::BButton) || keyboard->KeyDown(Key::B));
    }
    else if (button == "X")
    {
        return (pad->ButtonDown(GamePad::XButton) || keyboard->KeyDown(Key::X));
    }
    else if (button == "Y")
    {
        return (pad->ButtonDown(GamePad::YButton) || keyboard->KeyDown(Key::Y));
    }
    else if (button == "RB")
    {
        return (pad->ButtonDown(GamePad::RightShoulderButton) || keyboard->KeyDown(Key::S));
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void 
LobbyGameState::HandleHostRunningGame()
{
    // show all players in lobby
    const Ptr<Session>& session = MultiplayerManager::Instance()->GetSession();
    // list all session players
    const Util::Array< Ptr<Player> >& publicPlayers = session->GetPublicPlayers();
    IndexT i;
    for (i = 0; i < publicPlayers.Size(); ++i)
    {        
        this->RenderMenuEntry("PublicSlot: ", publicPlayers[i]->GetGamerTag().Value());
    }
    const Util::Array< Ptr<Player> >& privatePlayers = session->GetPrivatePlayers();
    for (i = 0; i < privatePlayers.Size(); ++i)
    {
        this->RenderMenuEntry("PrivateSlot: ", privatePlayers[i]->GetGamerTag().Value());
    }
    this->RenderMenuEntry("B", " - End Game");
    if (this->IsBtnAction("B"))
    {
        Multiplayer::MultiplayerManager::Instance()->EndGame();
    }

    this->HandleTestDataPackage();
}

//------------------------------------------------------------------------------
/**
*/
void 
LobbyGameState::HandleTestDataPackage()
{
    if (this->IsBtnAction("RB"))
    {
    }
}

} // namespace Application