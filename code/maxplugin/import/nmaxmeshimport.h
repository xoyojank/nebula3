//-----------------------------------------------------------------------------
//  nmaxmeshimport.h
//
//  (c)2005 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#ifndef N_MAXMESHIMPORT_H
#define N_MAXMESHIMPORT_H
//-----------------------------------------------------------------------------
/**
    @class nMaxMeshImport
    @ingroup NebulaMaxImportContrib

    @brief The Nebula2 3DS Max mesh importer.

    It only import vertex postion and uv0 of the vertex component.
*/
#include <Max.h>

//-----------------------------------------------------------------------------
class nMaxMeshImport
{
public:
    /// Constructor.
    nMaxMeshImport();
    /// Destructor.
    virtual ~nMaxMeshImport();

    /// Import Nebula mesh
    bool Import(const char* path, ImpInterface *i);

};
//-----------------------------------------------------------------------------
#endif
