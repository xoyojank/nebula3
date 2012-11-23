#pragma once
//------------------------------------------------------------------------------
/**
    @class Base::UIRendererBase
  
    Base class for internal UI rendering.
    
    (C) 2008 Radon Labs GmbH
*/    
#include "core/refcounted.h"
#include "core/singleton.h"
#include "math/rectangle.h"
#include "ui/rt/coreuielement.h"
#include "threading/threadid.h"
#include "resources/managedtexture.h"
#include "resources/resourcemanager.h"
#include "coregraphics/shaderinstance.h"
#include "util/stringatom.h"
#include "timing/timer.h"

//------------------------------------------------------------------------------
namespace Base
{
class CoreUIRendererBase : public Core::RefCounted
{
    __DeclareClass(CoreUIRendererBase);
    __DeclareSingleton(CoreUIRendererBase);
public:
    /// constructor
    CoreUIRendererBase();
    /// destructor
    virtual ~CoreUIRendererBase();
    
    /// open the the ui renderer
    void Setup();
    /// close the ui renderer
    void Discard();
    /// check if ui renderer open
    bool IsValid() const;

    /// set UI elements to render for this frame
    void SetElements(const Util::Array<CoreUI::CoreUIElement>& elements);
    /// draw the accumulated ui
    void DrawElements(const Ptr<CoreGraphics::ShaderInstance>& shdInst);
    /// load texture
    void LoadTexture(const Resources::ResourceId& resId);
    /// unload texture
    void UnloadTexture(const Resources::ResourceId& resId);
    /// check texture resources
    bool CheckTexturesLoaded() const;
    /// return number of elements in current frame
    SizeT GetNumElements() const;

protected:
    bool isValid;
    Util::Dictionary<int, CoreUI::CoreUIElement> sortedElements;
    Util::Dictionary<Resources::ResourceId, Ptr<Resources::ManagedTexture> > texturePool;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
CoreUIRendererBase::IsValid() const
{
    return this->isValid;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
CoreUIRendererBase::GetNumElements() const
{
    return this->sortedElements.Size();
}

} // namespace Base
//------------------------------------------------------------------------------
