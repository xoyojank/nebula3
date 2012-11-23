#pragma once
//------------------------------------------------------------------------------
/**
    @class Math::ClipStatus
    
    The result of a clip check (Inside, Outside or Clipped).
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Math
{
class ClipStatus
{
public:
    // clip status enum
    enum Type
    {
        Inside = 0,
        Outside = 1,
        Clipped = 2,

        Invalid,
    };
};

} // namespace Math    
//------------------------------------------------------------------------------
    