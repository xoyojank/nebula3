#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreUI::UIHandle
    
    A private Handle typedef to safely pass data between a server and
    a client thread. Don't assume anything about the content of the
    Handle!
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace CoreUI
{
typedef void* CoreUIHandle;
}
//------------------------------------------------------------------------------