#pragma once
//------------------------------------------------------------------------------
/**
    @class UI::UIRenderer
    
    Client-side proxy for rendering the user interface. Packs ui render requests
    into a message which is sent to the render thread once per frame.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "math/rectangle.h"
#include "ui/rt/coreuielement.h"

//------------------------------------------------------------------------------
namespace UI
{
class UIRenderer : public Core::RefCounted
{
    __DeclareClass(UIRenderer);
    __DeclareSingleton(UIRenderer);

public:
	/// constructor
    UIRenderer();
    /// destructor
    virtual ~UIRenderer();

    /// setup the UI renderer
    void Setup();
    /// discard the UI renderer
    void Discard();
    /// return true if UI renderer has been setup
    bool IsValid() const;

    /// draw a simple element
    void DrawElement(const Util::StringAtom& name, IndexT id, const Math::rectangle<float>& rect, const Math::float4& uv, int depth, const Math::float4& color, const Resources::ResourceId& texResId);
    /// call once per frame to send of accumulated draw commands
    void OnFrame();

    /// register a GUI texture
    void RegisterTexture(const Resources::ResourceId& resId);
    /// unregister a GUI texture
    void UnregisterTexture(const Resources::ResourceId& resId);

private:
    Util::Array<CoreUI::CoreUIElement> elements;
    Util::Dictionary<Resources::ResourceId,SizeT> texUseCounts;
    bool isValid;
}; 

//------------------------------------------------------------------------------
/**
*/
inline bool
UIRenderer::IsValid() const
{
    return this->isValid;
}

} // namespace UI
//------------------------------------------------------------------------------