//-----------------------------------------------------------------------------
//  nmaximporrt_script.cc
//
//  (c)2005 Kim, Hyoun Woo
//-----------------------------------------------------------------------------

#include <maxscrpt/maxscrpt.h>
#include <maxscrpt/parser.h>
#include <maxscrpt/definsfn.h>

#include "base/nmaxlistener.h"
#include "import/nmaxmeshimport.h"

//-----------------------------------------------------------------------------
/**
*/
Value* nImportMesh_cf(Value** arg_list, int count)
{
    // filename which to be imported into a scene.
    char* filename = arg_list[0]->to_string();
    if (!filename)
    {
        n_listener("Invalid filename.\n");
        return &undefined;
    }

#if 0
    //TODO: check the file extension and validate the file to import is
    //      Nebula mesh file type. 
    //      nString can not be used for this due to redefinition error.

    //FIXME: How do we get the pointer of ImpInterface?
    //       There is no way to get the pointer. :/
    ImpInterface *i = 0;

    nMaxMeshImport meshImport;
    if (!meshImport.Import(filename, i))
    {
        n_listener("Failed to import %s.", filename);
        return &undefined;
    }
#else
    n_listener("Scripted import command is not provided at the moment.\n");
#endif

    return &ok;
}

//-----------------------------------------------------------------------------
def_visible_primitive(nImportMesh, "nImportMesh");

