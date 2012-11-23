//------------------------------------------------------------------------------
//  testserverapplication.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/testserver/testserverapplication.h"
#include "io/binaryreader.h"
#include "io/binarywriter.h"
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
TestServerApplication::TestServerApplication()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
TestServerApplication::~TestServerApplication()
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
TestServerApplication::Open()
{
    n_assert(!this->IsOpen());
    if (ConsoleApplication::Open())
    {
        // setup a TcpServer object
        this->tcpServer = TcpServer::Create();
        this->tcpServer->SetAddress(IpAddress("any", 2352));
        if (this->tcpServer->Open())
        {
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
TestServerApplication::Close()
{
    n_assert(this->IsOpen());
    this->tcpServer->Close();
    ConsoleApplication::Close();
}

//------------------------------------------------------------------------------
/**
*/
void
TestServerApplication::Run()
{
    Ptr<BinaryReader> reader = BinaryReader::Create();
    Ptr<BinaryWriter> writer = BinaryWriter::Create();
    reader->SetStreamByteOrder(ByteOrder::Network);
    writer->SetStreamByteOrder(ByteOrder::Network);

    bool running = true;
    while (running)
    {
        Array<Ptr<TcpClientConnection>> recvConns = this->tcpServer->Recv();
        IndexT i;
        for (i = 0; i < recvConns.Size(); i++)
        {
            // get received data from current client
            reader->SetStream(recvConns[i]->GetRecvStream());
            reader->Open();
            String str = reader->ReadString();
            reader->Close();
            reader->SetStream(0);
            n_printf("Received '%s' from client '%s'\n", str.AsCharPtr(), recvConns[i]->GetClientAddress().GetHostName().AsCharPtr());

            // just reflect the received data back to the client
            writer->SetStream(recvConns[i]->GetSendStream());
            writer->Open();
            writer->WriteString(str);
            writer->Close();
            recvConns[i]->Send();

            if ("shutdown" == str)
            {
                running = false;
            }
        }
        n_sleep(0.001);
    }
}

} // namespace Tools