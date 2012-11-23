//-----------------------------------------------------------------------------
//  nmaxvectoranimator.h
//
//  (c)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#ifndef N_MAXVECTORANIMATOR_H
#define N_MAXVECTORANIMATOR_H
//-----------------------------------------------------------------------------
/**
    @class nMaxVectorAnimator
    @ingroup NebulaMaxExport2Contrib

    @brief A class for exporting nVectorAnimator.
*/
#include "export2/nmaxnode.h"

class nAnimator;

//-----------------------------------------------------------------------------
class nMaxVectorAnimator : public nMaxNode
{
public:
    /// Constructor.
    nMaxVectorAnimator();
    /// Destructor.
    virtual ~nMaxVectorAnimator();

    /// Function to export point4 type values.
    nAnimator* Export(const char* paramName, Control* control);
    /// Function to export floattab type, mostly four float type values.
    nAnimator* Export(const char* paramName, Control* control_x, Control* control_y, Control* control_z, Control* control_w);

};
//-----------------------------------------------------------------------------
#endif
