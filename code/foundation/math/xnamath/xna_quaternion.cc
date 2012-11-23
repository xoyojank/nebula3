//------------------------------------------------------------------------------
//  xna_quaternion.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "math/float4.h"
#include "math/matrix44.h"
#include "system/byteorder.h"

namespace Math
{

//------------------------------------------------------------------------------
/**
*/
quaternion
quaternion::rotationmatrix(const matrix44& m)
{
    return XMQuaternionRotationMatrix(m.mx);
}


}

