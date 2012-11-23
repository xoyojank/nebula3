#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreUI::CoreUIElement
    
    This is the container for the message wich is sent from the client side
	UI::UIRenderer to the CoreUI::UIGraphicsHandler and so on
    
    (C) 2008 Radon Labs GmbH
*/
#include "math/rectangle.h"
#include "resources/resourceid.h"

//------------------------------------------------------------------------------
namespace CoreUI
{
class CoreUIElement
{
public:
	/// constructor
	CoreUIElement();
	/// deconstructor
	~CoreUIElement();

    /// set name (for debugging)
    void SetName(const Util::StringAtom& name);
    /// get name
    const Util::StringAtom& GetName() const;
	/// set the color
	void SetColor(const Math::float4& c);
	/// get the color
	const Math::float4& GetColor() const;
	/// set the uv coordinates
	void SetUV(const Math::float4& uv);
	/// get the uv coordinates
	const Math::float4& GetUV() const;
	/// set the dimension
	void SetDimension(const Math::rectangle<float>& r);
	/// get the dimension
	const Math::rectangle<float>& GetDimension() const;
	/// set the depth 
	void SetDepth(int d);
	/// get the depth
	int GetDepth() const;
    /// set element id
    void SetElementId(IndexT i);
    /// get element id
    IndexT GetElementId() const;
    /// set texture resource id
    void SetTextureResId(const Resources::ResourceId& resId);
    /// get texture resource id
    const Resources::ResourceId& GetTextureResId() const;

private: 
    Util::StringAtom name;
    IndexT elementId;            
	Math::float4 color;	
	Math::float4 uv;
	Math::rectangle<float> dimension;
	int depth;
    Resources::ResourceId texResId;
};

//------------------------------------------------------------------------------
/**
*/
inline void
CoreUIElement::SetName(const Util::StringAtom& n)
{
    this->name = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
CoreUIElement::GetName() const
{
    return this->name;
}

//------------------------------------------------------------------------------
/**
*/
inline int
CoreUIElement::GetDepth() const
{
    return this->depth;
}

//------------------------------------------------------------------------------
/**
*/
inline void
CoreUIElement::SetDepth(int d)
{
	this->depth = d;
}

//------------------------------------------------------------------------------
/**
*/
inline void
CoreUIElement::SetUV(const Math::float4& u)
{
	this->uv = u;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::float4&
CoreUIElement::GetUV() const
{
	return this->uv;
}

//------------------------------------------------------------------------------
/**
*/
inline void
CoreUIElement::SetColor(const Math::float4& c)
{
	this->color = c;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::float4&
CoreUIElement::GetColor() const
{
	return this->color;
}

//------------------------------------------------------------------------------
/**
*/
inline void
CoreUIElement::SetDimension(const Math::rectangle<float>& r)
{
	this->dimension = r;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::rectangle<float>&
CoreUIElement::GetDimension() const
{
	return this->dimension;
}

//------------------------------------------------------------------------------
/**
*/
inline void
CoreUIElement::SetElementId(IndexT id)
{
    this->elementId = id;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT 
CoreUIElement::GetElementId() const
{
    return this->elementId;
}

//------------------------------------------------------------------------------
/**
*/
inline void
CoreUIElement::SetTextureResId(const Resources::ResourceId& resId)
{
    this->texResId = resId;
}

//------------------------------------------------------------------------------
/**
*/
inline const Resources::ResourceId&
CoreUIElement::GetTextureResId() const
{
	return this->texResId;
}

} // namespace CoreUI
//------------------------------------------------------------------------------
    