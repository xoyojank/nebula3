//-----------------------------------------------------------------------------
//  nmaximport.h
//
//  (c)2005 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#ifndef N_MAXIMPORT_H
#define N_MAXIMPORT_H
//-----------------------------------------------------------------------------
/**
    @class nMaxImport
    @ingroup NebulaMaxImportContrib

    @brief The Nebula2 3DS Max importer.

*/
#include <Max.h>
#include "base/nmaxdll.h"

#define N_MAX_IMPORT_CLASS_ID    Class_ID(0x47d96fb2, 0x1b981244)

#define LONGDESC     "Nebula2 Mesh Importer"
#define SHORTDESC    "Nebula2 Importer"
#define AUTHORNAME   "Dengwenyi"
#define COPYRIGHT    "Nebula License"
#define MESSAGE1     "This file is licensed under the terms of the Nebula License"
#define MESSAGE2     "(c) 2010 dengwenyi@gmail.com"
#define NUM_EXT	     5
#define VERSION	     100 // Version number * 100

//-----------------------------------------------------------------------------
class nMaxImport : public SceneImport
{
public:
    /// Constructor.
    nMaxImport();
    /// Destructor.
    virtual ~nMaxImport();

    /// @name Overridden functions from 3dsmax
    /// @{
    int	DoImport(const TCHAR *name, ImpInterface *i, Interface *gi, BOOL suppress=FALSE);
    /// Number of extensions supported
    int	ExtCount() { return NUM_EXT; }
    /// Extension #n (i.e. "3DS")
    const TCHAR * Ext(int n);
    /// Long ASCII description (i.e. "Autodesk 3D Studio File")
    const TCHAR * LongDesc() { return LONGDESC; }
    /// Short ASCII description (i.e. "3D Studio")
    const TCHAR * ShortDesc() { return SHORTDESC; }
    /// ASCII Author name
    const TCHAR * AuthorName() { return AUTHORNAME; }
    /// Copyright message
    const TCHAR * CopyrightMessage() { return COPYRIGHT; }
    /// message1
    const TCHAR * OtherMessage1() { return MESSAGE1; }
    /// message2
    const TCHAR * OtherMessage2() { return MESSAGE2; }
    /// Version number
    unsigned int Version() { return VERSION; }
    /// About dialog
    void ShowAbout(HWND hWnd) { return; }
    /// @}

protected:
    bool IsMeshFile(const nString &filename);

};
//-----------------------------------------------------------------------------
inline
const TCHAR*
nMaxImport::Ext(int n)
{
    switch (n)
    {
    case 0:
        return "n3d2";
    case 1:
        return "n3d";
    case 2:
        return "nvx2";
    case 3:
        return "nvx";
    case 4:
        return "obj";
    }
    return "";
}
//-----------------------------------------------------------------------------
static tRegisteredClassDesc<nMaxImport> maxImportInterfaceClassDesc2(
    "nMaxImport",          // class name
    N_MAX_IMPORT_CLASS_ID, // class ID
    SCENE_IMPORT_CLASS_ID, // super class ID
    1);//,                 // 1: public, 0: non-public
    //_T(""),              // category
    //"nebula2");          // scripter-visible name

#endif
