//-----------------------------------------------------------------------------
//  nmaxfloatanimator.h
//
//  (c)2004 Ben Wooller
//-----------------------------------------------------------------------------
#ifndef N_MAXUVANIMATOR_H
#define N_MAXUVANIMATOR_H
//-----------------------------------------------------------------------------
/**
    @class nMaxUVAnimator
    @ingroup NebulaMaxExport2Contrib

    @brief A class for exporting nUvAnimator.
*/
#include "export2/nmaxnode.h"

class nAnimator;

//-----------------------------------------------------------------------------
class nMaxUVAnimator : public nMaxNode
{
public:
    nMaxUVAnimator();
    virtual ~nMaxUVAnimator();

    nAnimator* Export(Texmap* texmap);

protected:
    Control* GetControlFromAnimatable(Animatable* anim, char* name);

};
//-----------------------------------------------------------------------------
#endif
