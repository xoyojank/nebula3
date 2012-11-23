//-----------------------------------------------------------------------------
//  nmaxtransformcurveanimator.h
//
//  (C)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#ifndef N_MAXTRANSFORMCURVEANIMATOR_H
#define N_MAXTRANSFORMCURVEANIMATOR_H
//-----------------------------------------------------------------------------
/**
    @class nMaxTransformCurveAnimator
    @ingroup NebulaMaxExport2Contrib

    @brief A class for handling path in 3dsmax.
           (This handles procedure controllers of 3dsmax)
*/
#include "export2/nmaxnode.h"

//-----------------------------------------------------------------------------
class nMaxTransformCurveAnimator : public nMaxNode
{
public:
    nMaxTransformCurveAnimator();
    virtual ~nMaxTransformCurveAnimator();

    virtual void Export(Control *control);
protected:
    

};
//-----------------------------------------------------------------------------
#endif
