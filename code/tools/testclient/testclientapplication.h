#pragma once
#ifndef TOOLS_TESTCLIENTAPPLICATION_H
#define TOOLS_TESTCLIENTAPPLICATION_H
//------------------------------------------------------------------------------
/**
    @class Tools::TestClientApplication
  
    Test TcpClient functionality.
    
    (C) 2006 Radon Labs GmbH
*/    
#include "app/consoleapplication.h"
#include "net/tcpclient.h"
#include "io/binaryreader.h"
#include "io/binarywriter.h"

//------------------------------------------------------------------------------
namespace Tools
{
class TestClientApplication : public App::ConsoleApplication
{
public:
    /// constructor
    TestClientApplication();
    /// destructor
    virtual ~TestClientApplication();
    /// open the application
    virtual bool Open();
    /// close the application
    virtual void Close();
    /// run the application, return when user wants to exit
    virtual void Run();

private:
    Ptr<Net::TcpClient> tcpClient;
    Ptr<IO::BinaryReader> reader;
    Ptr<IO::BinaryWriter> writer;
};

} // namespace Tools
//------------------------------------------------------------------------------
#endif

