#pragma once
//------------------------------------------------------------------------------
/**
    @class UI::Label

    A label represents a simple backgroundplane that contains a graphic and
    can have a tooltip set.
    
    (C) 2005 Radon Labs GmbH
*/
#include "ui/elements/element.h"

//------------------------------------------------------------------------------
namespace UI
{
class Label : public Element
{
	__DeclareClass(Label);
public:
    /// constructor
    Label();
    /// destructor
    virtual ~Label();
    /// called when gui hierarchy is created
    virtual void OnCreate();
};
}; // namespace UI
//------------------------------------------------------------------------------