#pragma once
//------------------------------------------------------------------------------
/**
    @class UI::DragBoxDisplay
    
    DragBoxDisplay UI element (currently placeholder).
    
    (C) 2010 Radon Labs GmbH
*/
#include "ui/elements/window.h"

//------------------------------------------------------------------------------
namespace UI
{
class DragBoxDisplay : public Window
{
    __DeclareClass(DragBoxDisplay);
public:
    /// constructor
    DragBoxDisplay();
    /// destructor
    virtual ~DragBoxDisplay();    
};

} // namespace UI
//------------------------------------------------------------------------------
    