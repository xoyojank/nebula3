//-----------------------------------------------------------------------------
//  nmaximport_main.cc
//
//  (c)2005 Kim, Hyoun Woo
//-----------------------------------------------------------------------------

#include "import/nmaximport.h"
#include "import/nmaxmeshimport.h"


//------------------------------------------------------------------------------
/**
*/
bool nMaxPluginInitialize()
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void nMaxPluginUninitialize()
{
}

//-----------------------------------------------------------------------------
/**
*/
nMaxImport::nMaxImport()
{
}

//-----------------------------------------------------------------------------
/**
*/
nMaxImport::~nMaxImport()
{
}

//-----------------------------------------------------------------------------
/**
*/
int nMaxImport::DoImport(const TCHAR *name, ImpInterface *i, Interface *gi, BOOL suppress)
{
    // load nebula mesh file.
    nString filename = name;

    if (IsMeshFile(filename))
    {
        nMaxMeshImport meshImport;
        return (meshImport.Import(filename.Get(), i) ? 1 : 0);
    }
    
    return 1;
}

//-----------------------------------------------------------------------------
/**
    True, if the file to be imported is mesh file type.
*/
bool nMaxImport::IsMeshFile(const nString &filename)
{
    if (filename.CheckExtension("n3d2") ||
        filename.CheckExtension("n3d")  ||
        filename.CheckExtension("nvx2") || 
        filename.CheckExtension("nvx")  ||
        filename.CheckExtension("obj") )
        return true;

    return false;
}

