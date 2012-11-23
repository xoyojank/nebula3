#pragma once
#ifndef MATH_VECTOR_H
#define MATH_VECTOR_H
//------------------------------------------------------------------------------
/**
    @class Math::vector
    
    A vector in homogenous space. A point describes a direction and length
    in 3d space and always has a w component of 0.0.
    
    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__ || __XBOX360__
#include "math/xnamath/xna_vector.h"
#elif __WII__
#include "math/wii/wii_vector.h"
#elif __PS3__
#include "math/ps3/ps3vector.h"
#else
#error "vector class not implemented!"
#endif
//------------------------------------------------------------------------------
#endif
