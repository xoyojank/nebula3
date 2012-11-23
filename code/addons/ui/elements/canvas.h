#pragma once
//------------------------------------------------------------------------------
/**
    @class UI::Canvas

    A canvas is the top level object of a ui hierarchy.
    
    (C) 2005 Radon Labs GmbH
*/
#include "ui/elements/element.h"

//------------------------------------------------------------------------------
namespace UI
{
class Canvas : public Element
{
	__DeclareClass(Canvas);
public:

    /// constructor
    Canvas();
    /// destructor
    virtual ~Canvas();
    /// set gfx resource name
    void SetResourceName(const Util::String& n);
    /// get gfx resource name
    const Util::String& GetResourceName() const;
    /// called when gui hierarchy is created
    virtual void OnCreate();
    /// called when gui hierarchy is destroyed
    virtual void OnDestroy();
    /// called before the gui hierarchy is rendered
    virtual void OnRender();
    /// called on frame
    virtual void OnFrame();

private:
    Util::String resourceName;

};

//------------------------------------------------------------------------------
/**
*/
inline void
Canvas::SetResourceName(const Util::String& n)
{
    this->resourceName = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String&
Canvas::GetResourceName() const
{
    return this->resourceName;
}

}; // namespace UI
//------------------------------------------------------------------------------