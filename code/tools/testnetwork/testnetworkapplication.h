#pragma once
#ifndef TOOLS_TESTNETWORKAPPLICATION_H
#define TOOLS_TESTNETWORKAPPLICATION_H
//------------------------------------------------------------------------------
/**
    @class Tools::TestNetworkApplication
    
    (C) 2006 Radon Labs GmbH
*/    
#include "app/consoleapplication.h"
#include "multiplayer/multiplayermanager.h"

//------------------------------------------------------------------------------
namespace Tools
{
class TestNetworkApplication : public App::ConsoleApplication
{
public:
    /// constructor
    TestNetworkApplication();
    /// destructor
    virtual ~TestNetworkApplication();
    /// open the application
    virtual bool Open();
    /// close the application
    virtual void Close();
    /// run the application, return when user wants to exit
    virtual void Run();

private:
    /// print user info text
    void PrintInfoText();
    /// handle cur app state returns exit
    bool HandleInput(const Util::String& str);
    /// handle state
    void HandleState();

    enum AppState
    {
        Init,
        Connect,
        WaitingForConnection,
        Sendstring,
        Idle
    };
    AppState curState;

    Ptr<Multiplayer::MultiplayerManager> multiplayerManager;
};

} // namespace Tools
//------------------------------------------------------------------------------
#endif

