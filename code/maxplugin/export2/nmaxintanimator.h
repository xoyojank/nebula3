//-----------------------------------------------------------------------------
//  nmaxintanimator.h
//
//  (c)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#ifndef N_MAXINTANIMATOR_H
#define N_MAXINTANIMATOR_H
//-----------------------------------------------------------------------------
/**
    @class nMaxIntAnimator
    @ingroup NebulaMaxExport2Contrib

    @brief A class for exporting nIntAnimator.
*/
#include "export2/nmaxnode.h"

class nAnimator;

//-----------------------------------------------------------------------------
class nMaxIntAnimator : public nMaxNode
{
public:
    nMaxIntAnimator();
    virtual ~nMaxIntAnimator();

    nAnimator* Export(const char* paramName, Control* control);

};
//-----------------------------------------------------------------------------
#endif
