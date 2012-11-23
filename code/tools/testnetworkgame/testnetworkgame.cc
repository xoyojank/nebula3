//------------------------------------------------------------------------------
//  testviewer.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/testnetworkgame/testnetworkgameapplication.h"

ImplementNebulaApplication()

using namespace Tools;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
void
NebulaMain(const CommandLineArgs& args)
{
    TestNetworkGameApplication app;
    app.SetCompanyName("Radon Labs GmbH");
    app.SetAppTitle("Test Network Game");
    app.SetCmdLineArgs(args);
    if (app.Open())
    {
        app.Run();
        app.Close();
    }
    app.Exit();
}