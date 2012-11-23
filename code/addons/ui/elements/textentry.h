#pragma once
//------------------------------------------------------------------------------
/**
    @class UI::TextEntry

    A TextEntry UI element. Currently a placeholder.
    
    (C) 2005 Radon Labs GmbH
*/
#include "ui/elements/element.h"

//------------------------------------------------------------------------------
namespace UI
{
class TextEntry : public Element
{
	__DeclareClass(TextEntry);
public:
    /// constructor
    TextEntry();
    /// destructor
    virtual ~TextEntry();
};

} // namespace UI
//------------------------------------------------------------------------------