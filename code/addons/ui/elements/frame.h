#pragma once
//------------------------------------------------------------------------------
/**
    @class UI::Frame
    
    A Frame object is used to group other elements together.
    
    (C) 2010 Radon Labs GmbH
*/
#include "ui/elements/element.h"

//------------------------------------------------------------------------------
namespace UI
{
class Frame : public Element
{
    __DeclareClass(Frame);
public:
    /// constructor
    Frame();
    /// destructor
    virtual ~Frame();
	/// return true if mouse is within element area
    virtual bool Inside(const Math::float2& mousePos);
};        

} // namespace UI
//------------------------------------------------------------------------------
    