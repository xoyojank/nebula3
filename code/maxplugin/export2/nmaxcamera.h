//---------------------------------------------------------------------------
//  nmaxcamera.cc
//
//  (c)2004 Kim, Hyoun Woo
//---------------------------------------------------------------------------
#ifndef N_MAXCAMERA_H
#define N_MAXCAMERA_H
//---------------------------------------------------------------------------
/**
    @class nMaxCamera
    @ingroup NebulaMaxExport2Contrib

    @brief A class for exporting camera data of 3DS Max.

*/
#include "mathlib/vector.h"
#include "export2/nmaxnode.h"

//---------------------------------------------------------------------------
class nMaxCamera : public nMaxNode
{
public:
    nMaxCamera();
    virtual ~nMaxCamera();

    void Export(INode* inode, Object *obj);

    // @name Functions for viewer.
    // @{
    void ExtractFromViewport();

    const vector3& GetEyePos() const;
    const vector3& GetEyeDir() const;
    const vector3& GetEyeUp() const;
    // @}

protected:
    vector3 eyepos;
    vector3 eyedir;
    vector3 eyeup;

};
//---------------------------------------------------------------------------
inline
const vector3& 
nMaxCamera::GetEyePos() const
{
    return this->eyepos;
}
//---------------------------------------------------------------------------
inline
const vector3& 
nMaxCamera::GetEyeDir() const
{
    return this->eyedir;
}
//---------------------------------------------------------------------------
inline
const vector3& 
nMaxCamera::GetEyeUp() const
{
    return this->eyeup;
}
//---------------------------------------------------------------------------
#endif
