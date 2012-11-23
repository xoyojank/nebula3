//-----------------------------------------------------------------------------
//  nmaxnode.h
//
//  (c)2004 Kim, Hyoun Woo.
//-----------------------------------------------------------------------------
#ifndef N_MAXNODE_H
#define N_MAXNODE_H
//-----------------------------------------------------------------------------
/**
    @class nMaxNode
    @ingroup NebulaMaxExport2Contrib

    @brief Base class of Nebula max object class which correspondes to each of
           the 3DS Max object.

    You can extend the exporter with creating a new class which derives this
    nMaxNode base class to export a 3DS Max object which is not supported by
    the exporter.

*/
//class nSceneNode;
#include <models/modelnode.h>

//-----------------------------------------------------------------------------
class nMaxNode
{
public:
    nMaxNode();
    virtual ~nMaxNode();


protected:
	virtual Models::ModelNode* CreateNebulaObject(const char* classname, const char* objectname);

};
//-----------------------------------------------------------------------------
#endif