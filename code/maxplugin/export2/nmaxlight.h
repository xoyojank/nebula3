//---------------------------------------------------------------------------
//  nmaxlight.h
//
//  (c)2004 Kim, Hyoun Woo
//---------------------------------------------------------------------------
#ifndef N_MAXLIGHT_H
#define N_MAXLIGHT_H
//---------------------------------------------------------------------------
/**
    @class nMaxLight
    @ingroup NebulaMaxExport2Contrib

    @brief

*/
#include "export2/nmaxnode.h"

class nLightNode;

//---------------------------------------------------------------------------
class nMaxLight : public nMaxNode
{
public:
    nMaxLight();
    virtual ~nMaxLight();

    nSceneNode* Export(INode* inode, Object* obj);

protected:
    bool BuildLight(Object *obj, nLightNode* light);

};
//---------------------------------------------------------------------------
#endif
