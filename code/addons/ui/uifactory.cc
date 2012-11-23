//------------------------------------------------------------------------------
//  ui/uifactory.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "ui/uifactory.h"
#include "ui/elements/canvas.h"
#include "ui/elements/button.h"
#include "ui/elements/frame.h"
#include "ui/elements/label.h"

namespace UI
{
__ImplementClass(UIFactory, 'UIFA', Core::RefCounted);
__ImplementSingleton(UIFactory);

//------------------------------------------------------------------------------
/**
*/
UIFactory::UIFactory()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
UIFactory::~UIFactory()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
    Create a user interface element from a type string.
*/
Ptr<Element>
UIFactory::CreateElement(const Util::String& type) const
{
	if (type == "Canvas")               return Canvas::Create();
    else if (type == "Frame")           return Frame::Create();              
    else if (type == "Button")          return Button::Create();
    else if (type == "Label")           return Label::Create();
    else
    {
		n_error("Invalid UI element type: '%s'!", type.AsCharPtr());
        return 0;
    }
}

}; // namespace UI
