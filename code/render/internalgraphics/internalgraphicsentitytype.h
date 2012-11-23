#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalGraphics::InternalGraphicsEntityType
    
    Defines graphics entity types.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace InternalGraphics
{
class InternalGraphicsEntityType
{
public:
    /// enumeration
    enum Code
    {
        Model = 0,
        Light,
        Camera,

        NumTypes,
        InvalidType,
    };
};

} // namespace InternalGraphics
//------------------------------------------------------------------------------
    