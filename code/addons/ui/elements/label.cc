//------------------------------------------------------------------------------
//  label.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "label.h"

namespace UI
{
__ImplementClass(UI::Label, 'UILA', UI::Element);

//------------------------------------------------------------------------------
/**
*/
Label::Label() 
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Label::~Label()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This method is called when the element has been attached to a parent.
*/
void
Label::OnCreate()
{
    // call parent class
    Element::OnCreate();
}
}; // namespace UI
