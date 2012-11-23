//------------------------------------------------------------------------------
//  idlc.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/idlcompiler/idlcompiler.h"

using namespace Tools;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
void __cdecl
main(int argc, const char** argv)
{
    CommandLineArgs args(argc, argv);
    IDLCompiler app;
    app.SetCompanyName("Radon Labs GmbH");
    app.SetAppTitle("Nebula3 IDL Compiler");
    app.SetCmdLineArgs(args);
    if (app.Open())
    {
        app.Run();
        app.Close();
    }
    app.Exit();
}
