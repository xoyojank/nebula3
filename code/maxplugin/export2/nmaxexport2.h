//-----------------------------------------------------------------------------
//  nmaxexport2.h
//
//  (c)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#ifndef N_MAXEXPORT2_H
#define N_MAXEXPORT2_H

//-----------------------------------------------------------------------------
/**
    @class nMaxExport2
    @ingroup NebulaMaxExport2Contrib

    @brief A class for Nebula2 3DS Max export plugin. 

*/
#include <Max.h>
#include <decomp.h> //the decompose functions for matrix3

#include "base/nmaxdll.h"
#include "export2/nmaxinterface.h"

#define N_MAX_EXPORT_CLASS_ID	Class_ID(0x7d137500, 0x313d2329)

#define LONGDESC    "Nebula2 Scene Exporter"
#define SHORTDESC   "Nebula2 Exporter"
#define AUTHORNAME  "Dengwenyi"
#define COPYRIGHT   "Nebula License"
#define MESSAGE1    "This file is licensed under the terms of the Nebula License"
#define MESSAGE2    "(c) 2010 dengwenyi@gmail.com"
#define NUM_EXT	    1
#define EXTENSION   "n2"

#define VERSION	    400	  // Version number * 100

//------------------------------------------------------------------------------
class nMaxExport2 : public SceneExport
{
public:
    /// Constructor.
    nMaxExport2();
    /// Destructor.
    virtual ~nMaxExport2();

    /// @name Overridden functions from 3dsmax
    /// @{
    int	DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);
    void ShowAbout(HWND hWnd) { return; }
    const TCHAR * Ext(int n);
    int	ExtCount() { return NUM_EXT; }
    BOOL SupportsOptions(int /*ext*/, DWORD /*options*/) { return true; }
    unsigned int Version()			 { return VERSION;   }
    const TCHAR * LongDesc()         { return LONGDESC;  }
    const TCHAR * ShortDesc()        { return SHORTDESC; }
    const TCHAR * AuthorName()       { return AUTHORNAME;}
    const TCHAR * CopyrightMessage() { return COPYRIGHT; }
    const TCHAR * OtherMessage1()    { return MESSAGE1;  }
    const TCHAR * OtherMessage2()    { return MESSAGE2;  }
    /// @}

protected:
    ///
    void Close();

protected:
    ExpInterface* expInterface;

};
//------------------------------------------------------------------------------
// ClassDesc2 defination for 'nMaxExpose' class
static tRegisteredClassDesc<nMaxExport2> maxExportInterfaceClassDesc2(
    "nMaxExport2",         // class name
    N_MAX_EXPORT_CLASS_ID, // class ID
    SCENE_EXPORT_CLASS_ID, // super class ID
    1);//,                 // 1: public, 0: non-public
    //_T(""),              // category
    //"nebula2");          // scripter-visible name

//------------------------------------------------------------------------------
/**
    return the file extension that is used in the export filetype dropdown
*/
inline 
const TCHAR* 
nMaxExport2::Ext(int n)
{ 
    switch (n)
    {
    case 0:
        return EXTENSION;
        break;
    }
    return "";
}
#endif
