//------------------------------------------------------------------------------
//  drasa3testui.cc
//  (C) 2010 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tests/drasa3testui/drasa3testuiapplication.h"

ImplementNebulaApplication()

using namespace Test;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
void
NebulaMain(const CommandLineArgs& args)
{
    Drasa3TestUiApplication app;
    app.SetCompanyName("Radon Labs GmbH");
    app.SetAppTitle("Drasa3 Test UI");
    app.SetCmdLineArgs(args);
    if (app.Open())
    {
        app.Run();
        app.Close();
    }
    app.Exit();
}