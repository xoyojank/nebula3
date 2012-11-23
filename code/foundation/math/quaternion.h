#pragma once
#ifndef MATH_QUATERNION_H
#define MATH_QUATERNION_H
//------------------------------------------------------------------------------
/**
    @class Math::quaternion

    Nebula's quaternion class.

    (C) 2004 RadonLabs GmbH
*/
#if __WIN32__ || __XBOX360__
#include "math/xnamath/xna_quaternion.h"
#elif __WII__
#include "math/wii/wii_quaternion.h"
#elif __PS3__
#include "math/ps3/ps3quaternion.h"
#else
#error "quaternion class not implemented!"
#endif
//-------------------------------------------------------------------
#endif
