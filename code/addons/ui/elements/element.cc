//------------------------------------------------------------------------------
//  ui/elements/element.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "ui/elements/element.h"
#include "ui/elements/window.h"
#include "ui/uifactory.h"
#include "ui/uimanager.h"
#include "ui/uievent.h"
#include "ui/uirenderer.h"
#include "debugrender/debugrender.h"

namespace UI
{
__ImplementClass(Element, 'UIEL', Core::RefCounted);

using namespace Math;
//------------------------------------------------------------------------------
/**
*/
Element::Element(): 
    visible(true),                                          
    enabled(true),                                          
    isValid(false),                                         
    elementId(0),
    parentElement(0),                                       
    parentWindow(0),                                        
    color(0,0,0,1),                                  
    uv(0,0,1,1),                               
    screenSpaceRect(0,0,0,0),                     
    renderColor(0,0,0,1),
    renderUV(0,0,1,1),
    mouseOver(false),                                          
    clickThrough(false)
{                                                              
}                                                              
                                                               
//------------------------------------------------------------------------------
/**                                                            
*/                                                            
Element::~Element()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Called by parent to build the gui hierarchy.
*/
void
Element::AttachElement(const Ptr<Element>& elm)
{
	n_assert(elm.isvalid());
    this->childElements.Append(elm);
	elm->SetParentElement(Ptr<Element>(this));
	elm->SetParentWindow(this->parentWindow);
    elm->OnCreate();
}

//------------------------------------------------------------------------------
/**
    Erase a child element. This will invoke the OnDestroy() method on the
    child. The child will be destroyed
*/
void
Element::EraseElement(const Ptr<Element>& elm)
{
	n_assert(elm.isvalid());
    elm->OnDestroy();
	Util::Array<Ptr<Element> >::Iterator iter = this->childElements.Find(elm);
    n_assert(iter);
    this->childElements.Erase(iter);
}

//------------------------------------------------------------------------------
/**
    Remove a child element from the childElements array. This won't destroy the child.
*/
void
Element::RemoveElement(const Ptr<Element>& elm)
{
	n_assert(elm.isvalid());
    Util::Array<Ptr<Element> >::Iterator iter = this->childElements.Find(elm);
    n_assert(iter);
    this->childElements.Erase(iter);
}

//------------------------------------------------------------------------------
/**
    Provides access to a single child element by an index.
*/
const Ptr<Element>&
Element::GetChildElementAt(int index) const
{
    n_assert(index>=0 && (IndexT)index < this->childElements.Size());    
    return this->childElements[index];
}

//------------------------------------------------------------------------------
/**
    Find a child element by its id. Returns 0 if not found.
*/
Ptr<Element>
Element::FindChildElement(const Util::StringAtom& id) const
{
    IndexT i;
    for (i = 0; i < this->childElements.Size(); i++)
    {
        if (id == this->childElements[i]->GetId())
        {
            return this->childElements[i];
        }
    }
    // fallthrough: not found
    return Ptr<Element>(0);
}

//------------------------------------------------------------------------------
/**
    This method is called when the element has been attached to a parent.
*/
void
Element::OnCreate()
{
    n_assert(!this->IsValid());
    this->isValid = true;
    this->elementId = UIManager::Instance()->GetNewElementId();
    if (this->texture.IsValid())
    {
        UIRenderer::Instance()->RegisterTexture(this->texture);
    }
}

//------------------------------------------------------------------------------
/**
    This method is called when the element has been removed from its parent.
*/
void
Element::OnDestroy()
{
    if (this->IsValid())
    {
        // release focus if necessary
        if (this->IsFocusElement())
        {
            this->UnregisterAsFocusElement();
        }

        // unregister texture
        if (this->texture.IsValid())
        {
            UIRenderer::Instance()->UnregisterTexture(this->texture);
        }

        // destroy all children
        IndexT i;
        for (i = 0; i < this->childElements.Size(); i++)
        {
            this->childElements[i]->OnDestroy();
        }
        this->childElements.Clear();   
        
        // unlink neighbours
        for(int i = 0; i < NumDirections; ++i)
        {
            this->neighbours[i] = NULL;
        }

         // clear own parent pointer
        this->parentElement = 0;
        this->parentWindow = 0;
        this->isValid = false;
    }
}

//------------------------------------------------------------------------------
/**
    This method is called once per frame during UIServer::Trigger() after
    the input has been delivered to the gui.
*/
void
Element::OnFrame()
{
    // distribute to children
    if (this->IsVisible())
    {
        IndexT i;
        for (i = 0; i < this->childElements.Size(); i++)
        {
            this->childElements[i]->OnFrame();
        }
    }
}

//------------------------------------------------------------------------------
/**
    This method is called when the element should render itself.
*/
void
Element::OnRender()
{
    int depth = this->CalculateHierarchyDepth() + 1;
    
    Resources::ResourceId texResId = this->renderTexture;
    if (!texResId.IsValid())
    {
        texResId = this->GetParentWindow()->GetWindowTexture();
    }
    UIRenderer::Instance()->DrawElement(this->id,
                                        this->elementId, 
                                        this->screenSpaceRect, 
                                        this->renderUV, 
                                        depth, 
                                        this->renderColor,
                                        texResId);

    // distribute to children
    IndexT i;
    for (i = 0; i < this->childElements.Size(); i++)
    {
        // just if they are visibly
        if (this->childElements[i]->IsVisible())
        {
            this->childElements[i]->OnRender();
        }
    }

    // at end of frame, reset the MOUSEOVER state!!!
    this->SetMouseOver(false);
}

//------------------------------------------------------------------------------
/**
    This method is called when the element should render itself in debug mode.
	(Just the shapes)
*/
void
Element::OnRenderDebug()
{
    // FIXME
}

//------------------------------------------------------------------------------
/**
    This method returns true if the given mouse position is inside the
    elements screen space area. By default this checks the mouse position
    against the screen space rectangle,
    but you can override the method in a subclass if you need more 
    advanced picking.
*/
bool
Element::Inside(const Math::float2& mousePos)
{
    if (this->IsVisible() && this->IsValid() && !this->IsClickThrough())
    {
        //rough check on screenSpaceRect
		bool inside =  this->screenSpaceRect.inside(mousePos.x(), mousePos.y());
        return inside;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**    
*/
int
Element::CalculateHierarchyDepth() const
{
	// calculate the depth of the element
	int depth = 0;
	Ptr<Element> p = this->parentElement;
	while(p.isvalid())
	{
		depth++;
		p = p->GetParentElement();
	}
	return depth + this->parentWindow->GetDepthOffset();
}

//------------------------------------------------------------------------------
/**    
*/
void
Element::Move(const Math::float2& vec)
{
    this->screenSpaceRect.left   += vec.x();
    this->screenSpaceRect.right  += vec.x();
    this->screenSpaceRect.top    += vec.y();
    this->screenSpaceRect.bottom += vec.y();

	// now call all childs to move
	for(IndexT i = 0; i < this->childElements.Size(); i++)
	{
		this->childElements[i]->Move(vec);
	}
}

//------------------------------------------------------------------------------
/**
    Checks all childs, appends them, if they are inside the position.
*/
void
Element::GetElementsAt(const Math::float2& mousePos, Util::Array<Ptr<Element> >& elements_out)
{
	// check all childs
	IndexT index;
	for(index = 0; index < this->childElements.Size(); index++)
	{
        if (this->childElements[index]->IsClickThrough())
        {
            // this is a click-through element, just recurse into children
			this->childElements[index]->GetElementsAt(mousePos, elements_out);
        }
        else if (this->childElements[index]->Inside(mousePos))
		{	
            elements_out.Append(this->childElements[index]);
			this->childElements[index]->GetElementsAt(mousePos, elements_out);
		}
	}
}

//------------------------------------------------------------------------------
/**
*/ 
void
Element::SetVisible(bool b)
{
    this->visible = b;
}

//------------------------------------------------------------------------------
/**
*/
bool
Element::IsVisible() const
{
    // recursively check if me or one of my parents is invisible
    if (!this->visible)
    {
        return false;
    }
	else if (this->GetParentElement().isvalid())
    {
        return this->GetParentElement()->IsVisible();
    }    
    else
    {
        return true;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Element::SetParentWindow(const Ptr<Window>& window)
{
	n_assert(window.isvalid());
	n_assert(!this->parentWindow.isvalid());
    
	this->parentWindow = window;    
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<Window>&
Element::GetParentWindow() const
{
    return this->parentWindow;
}

//------------------------------------------------------------------------------
/**
    If set to false the element won't call the OnX() methods anymore and doesn't
    distribute the input to it's children.
*/
bool
Element::IsEnabled() const
{
    // recursively check if me or one of my parents is disabled
    if (!this->enabled)
    {
        return false;
    }
	else if (this->GetParentElement().isvalid())
    {
        return this->GetParentElement()->IsEnabled();
    }    
    else
    {
        return true;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnMouseMove(const Math::float2& mousePos)
{
    // general setting for mouseover
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnLeftButtonDown(const Math::float2& mousePos)
{
    // implement in subclass   
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnLeftButtonPressed(const Math::float2& mousePos)
{
    // implement in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnLeftButtonUp(const Math::float2& mousePos)
{
    // implement in subclass    
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnLeftButtonDoubleClicked(const Math::float2& mousePos)
{
    // implement in subclass    
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnMiddleButtonDown(const Math::float2& mousePos)
{
    // implement in subclass    
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnMiddleButtonPressed(const Math::float2& mousePos)
{
    // implement in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnMiddleButtonUp(const Math::float2& mousePos)
{
    // implement in subclass    
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnMiddleButtonDoubleClicked(const Math::float2& mousePos)
{
    // implement in subclass    
}


//------------------------------------------------------------------------------
/**
*/
void
Element::OnRightButtonDown(const Math::float2& mousePos)
{
    // implement in subclass    
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnRightButtonPressed(const Math::float2& mousePos)
{
    // implement in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnRightButtonUp(const Math::float2& mousePos)
{
    // implement in subclass    
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnRightButtonDoubleClicked(const Math::float2& mousePos)
{
    // implement in subclass    
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnChar(const uchar& code)
{
    // implement in subclass    
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnKeyDown(Input::Key::Code key)
{
    // implement in subclass    
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnKeyUp(Input::Key::Code key)
{
    // implement in subclass    
}


//------------------------------------------------------------------------------
/**
*/
void
Element::OnGamePadButtonDown(Input::GamePad::Button btn)
{
    // implement in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnGamePadButtonUp(Input::GamePad::Button btn)
{
    // implement in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnFocusGain()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnFocusLost()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void 
Element::RegisterAsFocusElement()
{
    UI::UIManager::Instance()->SetFocusElement(Ptr<Element>(this), true);  
}

//------------------------------------------------------------------------------
/**
*/
void 
Element::UnregisterAsFocusElement()
{
    UI::UIManager::Instance()->SetFocusElement(Ptr<Element>(this), false);
}

//------------------------------------------------------------------------------
/**
*/
bool
Element::IsFocusElement() const
{
    return UI::UIManager::Instance()->GetFocusElement() == this;
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnMouseWheelMoved(const bool& up)
{
    // implement in subclass
}

//------------------------------------------------------------------------------
/**
*/
void 
Element::LinkNeighbours()
{
    if (this->parentElement.isvalid())
    {
        bool noNeighbours = true;
        const Util::Array<Ptr<Element> >& possibleNeighbours = this->parentElement->GetChildElements();
        const Math::rectangle<float>& ownRect = this->GetScreenSpaceRect();
        float2 ownRectCenter(ownRect.centerX(), ownRect.centerY());
        IndexT i;
        for (i = 0; i < possibleNeighbours.Size(); ++i)
        {
            if (possibleNeighbours[i].get_unsafe() != this)
            {
                const Math::rectangle<float>& otherRect = possibleNeighbours[i]->GetScreenSpaceRect();
                float2 otherRectCenter(otherRect.centerX(), otherRect.centerY());

                // check on left side
                if (otherRect.right < ownRect.left)
                {
                    if (this->neighbours[Left].isvalid())
                    {
                        const Math::rectangle<float>& preRect = this->neighbours[Left]->GetScreenSpaceRect();
                        float2 preRectCenter(preRect.centerX(), preRect.centerY());

                        float diffOld = n_abs(ownRectCenter.lengthsq() - otherRectCenter.lengthsq());
                        float diffNew = n_abs(ownRectCenter.lengthsq() - preRectCenter.lengthsq());
                        if (diffNew < diffOld)
                        {
                            this->neighbours[Left] = possibleNeighbours[i];
                        }
                    }
                    else
                    {
                        this->neighbours[Left] = possibleNeighbours[i];
                    }
                    noNeighbours = false;
                }
                // check right side
                else if (otherRect.left > ownRect.right)
                {
                    if (this->neighbours[Right].isvalid())
                    {
                        const Math::rectangle<float>& preRect = this->neighbours[Right]->GetScreenSpaceRect();
                        float2 preRectCenter(preRect.centerX(), preRect.centerY());

                        float diffOld = n_abs(ownRectCenter.lengthsq() - otherRectCenter.lengthsq());
                        float diffNew = n_abs(ownRectCenter.lengthsq() - preRectCenter.lengthsq());
                        if (diffNew < diffOld)
                        {
                            this->neighbours[Right] = possibleNeighbours[i];
                        }
                    }
                    else
                    {
                        this->neighbours[Right] = possibleNeighbours[i];
                    }
                    noNeighbours = false;
                }
                // check upper side
                if (otherRect.bottom < ownRect.top)
                {
                    if (this->neighbours[Top].isvalid())
                    {
                        const Math::rectangle<float>& preRect = this->neighbours[Top]->GetScreenSpaceRect();
                        float2 preRectCenter(preRect.centerX(), preRect.centerY());

                        float diffOld = n_abs(ownRectCenter.lengthsq() - otherRectCenter.lengthsq());
                        float diffNew = n_abs(ownRectCenter.lengthsq() - preRectCenter.lengthsq());
                        if (diffNew < diffOld)
                        {
                            this->neighbours[Top] = possibleNeighbours[i];
                        }
                    }
                    else
                    {
                        this->neighbours[Top] = possibleNeighbours[i];
                    }
                    noNeighbours = false;
                }
                // check bottom side
                else if (otherRect.top > ownRect.bottom)
                {
                    if (this->neighbours[Bottom].isvalid())
                    {
                        const Math::rectangle<float>& preRect = this->neighbours[Bottom]->GetScreenSpaceRect();
                        float2 preRectCenter(preRect.centerX(), preRect.centerY());

                        float diffOld = n_abs(ownRectCenter.lengthsq() - otherRectCenter.lengthsq());
                        float diffNew = n_abs(ownRectCenter.lengthsq() - preRectCenter.lengthsq());
                        if (diffNew < diffOld)
                        {
                            this->neighbours[Bottom] = possibleNeighbours[i];
                        }
                    }
                    else
                    {
                        this->neighbours[Bottom] = possibleNeighbours[i];
                    }
                    noNeighbours = false;
                }
            }
        }

        // warning if no neighbours
        if (noNeighbours)
        {
            n_printf("Element::LinkNeighbours() -> no neighbours found (element: %s)\n", this->GetId().Value());
        }
    }
}
} // namespace UI
