#pragma once
//------------------------------------------------------------------------------
/**
    @class UI::DragBox
    
    DragBox UI element (currently placeholder).
    
    (C) 2010 Radon Labs GmbH
*/
#include "ui/elements/element.h"

//------------------------------------------------------------------------------
namespace UI
{
class DragBox : public Element
{
    __DeclareClass(DragBox);
public:
    /// constructor
    DragBox();
    /// destructor
    virtual ~DragBox();    
};

} // namespace UI
//------------------------------------------------------------------------------
    