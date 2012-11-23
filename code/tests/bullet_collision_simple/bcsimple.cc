//------------------------------------------------------------------------------
//  ps3test.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "bcapplication.h"
#include "system/appentry.h"

ImplementNebulaApplication();

//------------------------------------------------------------------------------
/**
*/
void
NebulaMain(const Util::CommandLineArgs& args)
{
    Tools::BCApplication app;
    app.SetAppTitle("Bullet Collision Simple");
    app.SetAppID("BCSIMPLE");
    if (app.Open())
    {
        app.Run();
        app.Close();
    }    
}
