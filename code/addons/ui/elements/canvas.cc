//------------------------------------------------------------------------------
//  ui/elements/canvas.cc
//  (C) 2005 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "ui/elements/canvas.h"
#include "ui/elements/window.h"
#include "ui/uimanager.h"

namespace UI
{
__ImplementClass(Canvas, 'UICV', Element);

//------------------------------------------------------------------------------
/**
*/
Canvas::Canvas()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Canvas::~Canvas()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This creates a graphics entity representing the visual of the canvas
    and converts the resource name into a Nebula2 node pointer.
*/
void
Canvas::OnCreate()
{
    // canvases don't have parents
	n_assert(this->parentWindow.isvalid());
	n_assert(!this->parentElement.isvalid());
	Element::OnCreate();
}

//------------------------------------------------------------------------------
/**
*/
void
Canvas::OnDestroy()
{
    Element::OnDestroy();
}

//------------------------------------------------------------------------------
/**
*/
void
Canvas::OnRender()
{
    // don't render the canvas itself, only its child elements
    IndexT i;
    for (i = 0; i < this->childElements.Size(); i++)
    {
        if (this->childElements[i]->IsVisible())
        {
            this->childElements[i]->OnRender();
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Canvas::OnFrame()
{
    Element::OnFrame();    
}

} // namespace UI
