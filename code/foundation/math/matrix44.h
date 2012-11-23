#pragma once
#ifndef MATH_MATRIX44_H
#define MATH_MATRIX44_H
//------------------------------------------------------------------------------
/**
    @file math/matrix44.h

    Frontend header for matrix classes.

    (C) 2006 Radon Labs GmbH
*/
#if __WIN32__ || __XBOX360__
#include "math/xnamath/xna_matrix44.h"
#elif __WII__
#include "math/wii/wii_matrix44.h"
#elif __PS3__
#include "math/ps3/ps3matrix44.h"
#else
#error "matrix44 class not implemented!"
#endif
//-------------------------------------------------------------------
#endif
