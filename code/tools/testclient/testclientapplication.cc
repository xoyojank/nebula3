//------------------------------------------------------------------------------
//  testclientapplication.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/testclient/testclientapplication.h"
#include "io/console.h"
#include "system/byteorder.h"

namespace Tools
{
using namespace Util;
using namespace Net;
using namespace IO;
using namespace System;

//------------------------------------------------------------------------------
/**
*/
TestClientApplication::TestClientApplication()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
TestClientApplication::~TestClientApplication()
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
TestClientApplication::Open()
{
    n_assert(!this->IsOpen());

    // display help?
    if (this->args.HasArg("-help"))
    {
        n_printf("-help -- show this help\n"
                 "-host -- server hostname (default: localhost)\n");
        return false;
    }
    
    if (ConsoleApplication::Open())
    {
        IpAddress ipAddr;
        ipAddr.SetHostName(this->args.GetString("-host", "localhost"));
        ipAddr.SetPort(2352);

        // setup a tcp client object
        this->tcpClient = TcpClient::Create();
        this->tcpClient->SetBlocking(true);
        this->tcpClient->SetServerAddress(ipAddr);
        TcpClient::Result res = this->tcpClient->Connect();
        if (TcpClient::Success == res)
        {
            // setup stream reader and writer objects
            this->reader = BinaryReader::Create();
            this->reader->SetStream(this->tcpClient->GetRecvStream());
            this->reader->SetStreamByteOrder(ByteOrder::Network);
            this->writer = BinaryWriter::Create();
            this->writer->SetStream(this->tcpClient->GetSendStream());
            this->writer->SetStreamByteOrder(ByteOrder::Network);

            return true;
        }
        else
        {
            ConsoleApplication::Close();
            return false;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
TestClientApplication::Close()
{
    n_assert(this->isOpen);
    n_assert(this->tcpClient.isvalid());
    if (this->tcpClient->IsConnected())
    {
        this->tcpClient->Disconnect();
    }
    this->tcpClient = 0;

    ConsoleApplication::Close();
}

//------------------------------------------------------------------------------
/**
    This just takes input from the console and sends it to the server,
    which should just reflect it.
*/
void
TestClientApplication::Run()
{
    Console* con = Console::Instance();
    bool running = true;
    while (running)
    {
        con->Print("> ");
        String str = con->GetInput();
        if (str.IsValid())
        {
            if (str == "exit")
            {
                running = false;
            }
            else
            {
                // send user input to server
                this->writer->Open();
                this->writer->WriteString(str);
                this->writer->Close();
                if (this->tcpClient->Send())
                {
                    // get reply from server
                    if (this->tcpClient->Recv())
                    {
                        this->reader->Open();
                        String reply = this->reader->ReadString();
                        this->reader->Close();
                        n_printf("reply: %s\n", reply.AsCharPtr());
                    }
                    else
                    {
                        n_printf("Error in Recv().\n");
                        running = false;
                    }
                }
                else
                {
                    n_printf("Error during Send().\n");
                    running = false;
                }
            }
        }
    }
}

} // namespace Tools
