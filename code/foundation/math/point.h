#pragma once
#ifndef MATH_POINT_H
#define MATH_POINT_H
//------------------------------------------------------------------------------
/**
    @class Math::point
    
    A point in homogenous space. A point describes a position in space,
    and has its W component set to 1.0.
    
    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__ || __XBOX360__
#include "math/xnamath/xna_point.h"
#elif __WII__
#include "math/wii/wii_point.h"
#elif __PS3__
#include "math/ps3/ps3point.h"
#else
#error "point class not implemented!"
#endif
//------------------------------------------------------------------------------
#endif
    