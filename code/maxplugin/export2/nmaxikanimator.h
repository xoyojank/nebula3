//-----------------------------------------------------------------------------
//  nmaxikanimator.h
//
//  (C)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#ifndef N_MAXIKANIMATOR_H
#define N_MAXIKANIMATOR_H

//-----------------------------------------------------------------------------
/**
    @class nMaxIKAnimator
    @ingroup NebulaMaxExport2Contrib

    @brief A class for handling Inverse Kinemetics(IK).
*/
class nMaxTransformAnimator;

//-----------------------------------------------------------------------------
class nMaxIKAnimator : public nMaxTransformAnimator
{
public:
    nMaxIKAnimator();
    virtual ~nMaxIKAnimator();

    virtual void Export(INode *inode);
protected:
    

};
//-----------------------------------------------------------------------------
#endif
