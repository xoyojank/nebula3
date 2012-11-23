//------------------------------------------------------------------------------
//  testclient.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/testnetwork/testnetworkapplication.h"

using namespace Tools;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
void __cdecl
main(int argc, const char** argv)
{
    CommandLineArgs args(argc, argv);
    TestNetworkApplication app;
    app.SetCompanyName("Radon Labs GmbH");
    app.SetAppName("Test Network");
    app.SetCmdLineArgs(args);
    if (app.Open())
    {
        app.Run();
        app.Close();
    }
    app.Exit();
}
