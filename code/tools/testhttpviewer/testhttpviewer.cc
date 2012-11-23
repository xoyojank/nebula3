//------------------------------------------------------------------------------
//  testhttpviewer.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/testhttpviewer/testhttpviewerapplication.h"

ImplementNebulaApplication()

using namespace Tools;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
void
NebulaMain(const CommandLineArgs& args)
{
    TestHttpViewerApplication app;
    app.SetCompanyName("Radon Labs GmbH");
    app.SetAppTitle("Test HTTP Viewer");
    app.SetCmdLineArgs(args);
    app.SetLogFileEnabled(false);
    app.SetMountStandardArchivesEnabled(false);
    app.SetOverrideRootDirectory("httpnz://213.191.63.156/testhttpviewer");
    if (app.Open())
    {
        app.Run();
        app.Close();
    }
    app.Exit();
}