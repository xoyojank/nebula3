//------------------------------------------------------------------------------
//  TestNetworkApplication.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/testnetwork/testnetworkapplication.h"
#include "io/console.h"
#include "system/byteorder.h"
#include "corenetwork/netconfig.h"
#include "multiplayerprotocol.h"

namespace Tools
{
using namespace Util;
using namespace IO;
using namespace System;
using namespace Multiplayer;

//------------------------------------------------------------------------------
/**
*/
TestNetworkApplication::TestNetworkApplication():
curState(Init)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
TestNetworkApplication::~TestNetworkApplication()
{
    if (this->IsOpen())
    {
        this->Close();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
TestNetworkApplication::Open()
{
    n_assert(!this->IsOpen());

    // display help?
    if (this->args.HasArg("-help"))
    {
        n_printf("-help -- show this help\n");
        return false;
    }
    
    if (ConsoleApplication::Open())
    {
        this->multiplayerManager = Multiplayer::MultiplayerManager::Create();      
        this->multiplayerManager->Open();  
        return true;        
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
TestNetworkApplication::Close()
{
    n_assert(this->isOpen);

    ConsoleApplication::Close();
}

//------------------------------------------------------------------------------
/**
    This just takes input from the console and sends it to the server,
    which should just reflect it.
*/
void
TestNetworkApplication::Run()
{
    Console* con = Console::Instance();
    bool running = true;

    this->PrintInfoText();    
    while (running)
    {
        if (con->HasInput())
        {
            this->HandleInput(con->GetInput());
            this->PrintInfoText();
        }
        this->HandleState();
        
        this->multiplayerManager->OnFrame();
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
TestNetworkApplication::PrintInfoText()
{
    Console* con = Console::Instance();
    switch (curState)
    {
    case (Init): con->Print("'s' : start as server or 'c' : start as client\n");
        break;
    case (Connect):
        con->Print("Enter ip and port to connect to. "
            "Enter will connect to localhost at default server port. "
            "Or destroy client with 'k'\n");
        break;
    case (Sendstring):
        con->Print("Send any string to server or disconnect with 'd'.\n");
        break;
    }
    con->Print("> ");
}

//------------------------------------------------------------------------------
/**
*/
bool 
TestNetworkApplication::HandleInput(const Util::String& str)
{    
    if (str.IsValid())
    {
        if (str == "exit" || str == "q")
        {
            if (this->multiplayerManager->IsOpen())
            {
                this->multiplayerManager->StopDefaultService();
                this->multiplayerManager->Close();
            }
            return false;
        }
    }

    switch (curState)
    {
    case (Init): 
        if (str.IsValid() && str == "s")
        {
            // start server
            this->multiplayerManager->StartDefaultServerService();
            this->curState = Idle;
        }
        else if (str.IsValid() && str == "c")
        {
            // start client
            this->multiplayerManager->StartDefaultClientService();
            this->curState = Connect;
        }
        break;
    case (Connect):
        if (str.IsValid())
        {   
            if (str == "k")
            {
                this->multiplayerManager->Close();
                this->curState = Init;
            }
            else
            {
                // to do validate uri format
                if (str.FindCharIndex(':'))
                {
                    // connect to server
                    this->curState = WaitingForConnection;
                    IO::URI uri;
                    uri.SetHost(str.ExtractRange(0, str.FindCharIndex(':')));
                    uri.SetPort(str.ExtractToEnd(str.FindCharIndex(':')));
                    // send connect msg
                    Ptr<Multiplayer::Connect> connect = Multiplayer::Connect::Create();
                    connect->SetServerAddress(uri);
                    this->multiplayerManager->SendToServer(connect.cast<Messaging::Message>());
                }
                else
                {
                    n_printf("No valid server address!\n");
                }
            }
        }
        else
        {   
            // connect to server with default port
            this->curState = WaitingForConnection;
            IO::URI uri;
            uri.SetHost("localhost");
            uri.SetPort(String::FromInt(SERVER_DEFAULT_PORT));
            // send connect msg
            Ptr<Multiplayer::Connect> connect = Multiplayer::Connect::Create();
            connect->SetServerAddress(uri);
            this->multiplayerManager->SendToServer(connect.cast<Messaging::Message>());
        }
        break;

    case (WaitingForConnection):
        n_printf("Waiting for connection success!\n");
        break;
    case (Sendstring):
        if (str.IsValid() && str == "d")
        {   
            // send connect msg
            Ptr<Multiplayer::Disconnect> disconnect = Multiplayer::Disconnect::Create();            
            this->multiplayerManager->SendToServer(disconnect.cast<Messaging::Message>());
            // connect to server
            this->curState = Connect;
        }
        else if (str.IsValid())
        {
            Ptr<Multiplayer::Chatmsg> chatMsg = Multiplayer::Chatmsg::Create();
            chatMsg->SetText(str);
            this->multiplayerManager->SendToServer(chatMsg.cast<Messaging::Message>());
        }
        break;
    }
    return true;
}


//------------------------------------------------------------------------------
/**
*/
void TestNetworkApplication::HandleState()
{
    switch (this->curState)
    {    
    case (WaitingForConnection):
        {
            Ptr<Multiplayer::IsClientConnected> clientConnected = Multiplayer::IsClientConnected::Create();                     
            this->multiplayerManager->HandleSystemMessageBy(clientConnected.cast<Messaging::Message>(), MultiplayerManager::ServerMode);
            if (clientConnected->GetIsConnected())
            {
                this->curState = Sendstring;
            }
        }
        break;
    }
}
} // namespace Tools
