//------------------------------------------------------------------------------
//  testserver.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/testserver/testserverapplication.h"

using namespace Tools;
using namespace Util;

ImplementNebulaApplication();

//------------------------------------------------------------------------------
/**
*/
void
NebulaMain(const CommandLineArgs& args)
{
    TestServerApplication app;
    app.SetCompanyName("Radon Labs GmbH");
    app.SetAppTitle("Test Server");
    app.SetCmdLineArgs(args);
    if (app.Open())
    {
        app.Run();
        app.Close();
    }
    app.Exit();
}