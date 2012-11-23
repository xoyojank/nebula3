//------------------------------------------------------------------------------
//  nmaxutilityobj.cc
//
//  (c)2004 Kim, Hyoun Woo
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "pluginlibs/nmaxutilityobj.h"
#include "pluginlibs/nmaxoptionparammapdlg.h"
#include "../res/nmaxtoolbox.h"

IParamMap* nMaxUtilityObj::optionParamMap = 0;

//------------------------------------------------------------------------------
/**
*/
nMaxUtilityObj::nMaxUtilityObj()
{
    flags = ROLLUP_EXPORTOPTIONS_OPEN;
}

//------------------------------------------------------------------------------
/**
*/
nMaxUtilityObj::~nMaxUtilityObj()
{
}

//------------------------------------------------------------------------------
/**
    Create Nebula2 utility panel plugin to support options.

    - 21-Mar-05 kims changed to check nMaxOptionParamMapDlg is correctly created.
                     (used native 'new' instead of 'n_new' cause destroying its  
                      instance is depends on 3dsmax side)
*/
void nMaxUtilityObj::BeginEditParams(Interface* intf, IUtil* iutil)
{
    // create nebula utility panel dialog.
    optionParamMap = CreateCPParamMap(
        NULL, 
        0,//1,
        this, 
        intf, 
        maxUtilityObjInterfaceClassDesc2.HInstance(), 
        MAKEINTRESOURCE(IDD_UTILITY_OPTION),
        "Nebula Export Options",
        (flags & ROLLUP_EXPORTOPTIONS_OPEN) ? 0 : APPENDROLL_CLOSED);

    if (optionParamMap)
    {
        ParamMapUserDlgProc* paramMapDlgProc = new nMaxOptionParamMapDlg();

        if (paramMapDlgProc)
        {
            optionParamMap->SetUserDlgProc(paramMapDlgProc);
        }
        else
        {
            n_message("Failed to create option panel of utility.");
            return;
        }
    }
}

//------------------------------------------------------------------------------
/**
     Release created utility panel dialog when Nebula2 utility panel is closed.
*/
void nMaxUtilityObj::EndEditParams(Interface* intf, IUtil* iutil)
{
    if (optionParamMap)
    {
        // destroy nebula utility panel dialog.
        if (IsRollupPanelOpen(optionParamMap->GetHWnd()))
            flags |= ROLLUP_EXPORTOPTIONS_OPEN;
        DestroyCPParamMap(optionParamMap); 
        optionParamMap = NULL;
    }
}
