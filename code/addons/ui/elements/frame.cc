//------------------------------------------------------------------------------
//  frame.cc
//  (C) 2010 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "ui/elements/frame.h"

namespace UI
{
__ImplementClass(UI::Frame, 'UIFM', UI::Element);

using namespace Math;

//------------------------------------------------------------------------------
/**
*/
Frame::Frame()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Frame::~Frame()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Return true if mouse is over one of the child elements.
*/
bool
Frame::Inside(const float2& mousePos)
{
    if (this->IsVisible() && this->IsValid())
    {
        if (this->screenSpaceRect.inside(mousePos.x(), mousePos.y()))
        {
            IndexT i;
            for (i = 0; i < this->childElements.Size(); i++)
            {
                if (this->childElements[i]->Inside(mousePos))
                {
                    return true;
                }
            }
        }
    }
    // fallthrough: mouse not over any child element
    return false;
}

} // namespace UI