//------------------------------------------------------------------------------
//  texturestreamingviewer.cc
//  (C) 2010 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tests/texturestreamingtest_win32/texturestreamingapplication.h"

ImplementNebulaApplication()

using namespace Tests;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
void
NebulaMain(const CommandLineArgs& args)
{
    TextureStreamingApplication app;
    app.SetCompanyName("Radon Labs GmbH");
    app.SetAppTitle("Test Viewer");
    app.SetCmdLineArgs(args);
    if (app.Open())
    {
        app.Run();
        app.Close();
    }
    app.Exit();
}