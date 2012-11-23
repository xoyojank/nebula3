#pragma once
#ifndef TOOLS_TESTSERVERAPPLICATION_H
#define TOOLS_TESTSERVERAPPLICATION_H
//------------------------------------------------------------------------------
/**
    @class Tools::TestServerApplication
    
    For TcpServer functionality tests.
    
    (C) 2006 Radon Labs GmbH
*/
#include "app/consoleapplication.h"
#include "net/tcpserver.h"

//------------------------------------------------------------------------------
namespace Tools
{
class TestServerApplication : public App::ConsoleApplication
{
public:
    /// constructor
    TestServerApplication();
    /// destructor
    virtual ~TestServerApplication();
    /// open the application
    virtual bool Open();
    /// close the application
    virtual void Close();
    /// run the application, return when user wants to exit
    virtual void Run();

private:
    Ptr<Net::TcpServer> tcpServer;
};

} // namespace Tools
//------------------------------------------------------------------------------
#endif
    