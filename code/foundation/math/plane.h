#pragma once
#ifndef MATH_PLANE_H
#define MATH_PLANE_H
//------------------------------------------------------------------------------
/**
    @class Math::plane

    Nebula's plane class.

    (C) 2007 RadonLabs GmbH
*/
#if __WIN32__ || __XBOX360__
#include "math/xnamath/xna_plane.h"
#elif __WII__
#include "math/wii/wii_plane.h"
#elif __PS3__
#include "math/ps3/ps3plane.h"
#else
#error "plane class not implemented!"
#endif
//------------------------------------------------------------------------------
#endif
