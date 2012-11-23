#pragma once
#ifndef MATH_FLOAT4_H
#define MATH_FLOAT4_H
//------------------------------------------------------------------------------
/**
    @class Math::float4

    A 4-component float vector class. This is the basis class for points
    and vectors.
    
    (C) 2007 RadonLabs GmbH
*/
#if __WIN32__ || __XBOX360__
#include "math/xnamath/xna_float4.h"
#elif __WII__
#include "math/wii/wii_float4.h"
#elif __PS3__
#include "math/ps3/ps3float4.h"
#else
#error "float4 class not implemented!"
#endif
//------------------------------------------------------------------------------
#endif
