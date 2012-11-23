#pragma once
//------------------------------------------------------------------------------
/**
    @class UI::UIFactory
  
    Creates user interface elements.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"

//------------------------------------------------------------------------------
namespace UI
{
class Element;

class UIFactory : public Core::RefCounted
{
	__DeclareClass(UIFactory);
    __DeclareSingleton(UIFactory);
public:
    /// constructor
    UIFactory();
    /// destructor
    virtual ~UIFactory();

    /// create a user interface element from type string
	virtual Ptr<Element> CreateElement(const Util::String& type) const;
};

}; // namespace UI
//------------------------------------------------------------------------------
