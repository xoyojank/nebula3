//------------------------------------------------------------------------------
//  fmodtest.cc
//  (C) 2010 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "fmodtestapplication.h"

ImplementNebulaApplication();

using namespace Test;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
void
NebulaMain(const CommandLineArgs& args)
{
    FmodTestApplication app;
    app.SetCompanyName("Radon Labs GmbH");
    app.SetAppID("FMOD Test");
    app.SetCmdLineArgs(args);
    if (app.Open())
    {
        app.Run();
        app.Close();
    }
    app.Exit();
}