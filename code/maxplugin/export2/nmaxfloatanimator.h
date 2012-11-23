//-----------------------------------------------------------------------------
//  nmaxfloatanimator.h
//
//  (c)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#ifndef N_MAXFLOATANIMATOR_H
#define N_MAXFLOATANIMATOR_H
//-----------------------------------------------------------------------------
/**
    @class nMaxFloatAnimator
    @ingroup NebulaMaxExport2Contrib

    @brief A class for exporting nVectorAnimator.
*/
#include "export2/nmaxnode.h"

class nAnimator;

//-----------------------------------------------------------------------------
class nMaxFloatAnimator : public nMaxNode
{
public:
    nMaxFloatAnimator();
    virtual ~nMaxFloatAnimator();

    nAnimator* Export(const char* paramName, Control* control);

};
//-----------------------------------------------------------------------------
#endif
