//------------------------------------------------------------------------------
//  nmaxtoolbox.cc
//
//  (c)2004 Kim, Hyoun Woo.
//------------------------------------------------------------------------------
#include "base/nmaxdll.h"
#include "export2/nmaxoptions.h"
#include "pluginlibs/nmaxdlg.h"
#include "pluginlibs/nmaxlogdlg.h"

nNebulaUsePackage(nnebula);

//------------------------------------------------------------------------------
/**
    called when the plugin dll is loaded.

    -17-Aug-06  kims  Modified to read INI file before the plugin generates
                      max script code.
                      
*/
bool nMaxPluginInitialize()
{
    nKernelServer::Instance()->AddPackage(nnebula);

    // load settings at the startup time cause utility panel is independent
    // to the exporting task. (same reason for saving the utility options too)
    nMaxOptions* options = nMaxOptions::Instance();
    if (!options->LoadUtilityOptions())
    {
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
/**
    called when the plugin dll is unloaded.

    -02-Nov-06  kims  Fixed to close log dialog even if it is opened when we close 3dsmax.
*/
void nMaxPluginUninitialize()
{
    // release option module.
    nMaxOptions* options = nMaxOptions::Instance();
    if (options)
    {
        // save utility panel options.
        options->SaveUtilityOptions();

        n_delete(options);
    }

    // close log dialog if it is still alive.
    nMaxLogDlg* logDlg = nMaxLogDlg::Instance();
    if (logDlg)
    {
        n_delete(logDlg);
    }
}
