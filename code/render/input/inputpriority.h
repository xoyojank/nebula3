#pragma once
//------------------------------------------------------------------------------
/**
    @class Input::InputPriority
    
    Input priorities for input handlers.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Input
{
class InputPriority
{
public:
    enum Code
    {
        Gui = 0,
        Game,
        Other,
    };
};

} // namespace Input
//------------------------------------------------------------------------------

