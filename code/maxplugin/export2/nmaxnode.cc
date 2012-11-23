//-----------------------------------------------------------------------------
//  nmaxnode.cc
//
//  (c)2004 Kim, Hyoun Woo.
//-----------------------------------------------------------------------------
#include "stdneb.h"
#include "export2/nmax.h"
#include "export2/nmaxnode.h"
#include "export2/nmaxutil.h"
#include "pluginlibs/nmaxdlg.h"
#include "pluginlibs/nmaxlogdlg.h"

//#include "kernel/nkernelserver.h"
//#include "scene/nscenenode.h"

//-----------------------------------------------------------------------------
/**
*/
nMaxNode::nMaxNode()
{
}

//-----------------------------------------------------------------------------
/**
*/
nMaxNode::~nMaxNode()
{
}

//-----------------------------------------------------------------------------
/**
    Create Nebula object of the given class name and set the created object to
    current working directory.

    The created object has its name as 3dsmax node.

    @param nodeName 3dsmax node name which to be used for Nebula object's name
    @param classname Nebula object class name.

    @return pointer to the created Nebula object.
*/
Models::ModelNode* 
nMaxNode::CreateNebulaObject(const char* classname, const char* objectname)
{
    n_assert(classname);

    Models::ModelNode* sceneNode = 0;
	return sceneNode;
}
