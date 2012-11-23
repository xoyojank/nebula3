//------------------------------------------------------------------------------
//  uirendererbase.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "ui/base/coreuirendererbase.h"
#include "ui/rt/coreuielement.h"

namespace Base
{
__ImplementClass(CoreUIRendererBase, 'UIRB', Core::RefCounted);
__ImplementSingleton(CoreUIRendererBase);

using namespace Util;
using namespace Math;
using namespace Resources;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
CoreUIRendererBase::CoreUIRendererBase() : 
    isValid(false)
{
	__ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
CoreUIRendererBase::~CoreUIRendererBase()
{
	n_assert(!this->IsValid());
	__DestructSingleton
}

//------------------------------------------------------------------------------
/**
*/
void
CoreUIRendererBase::Setup()
{
	n_assert(!this->IsValid());
    this->isValid = true;

    // initialize dummy texture for "no-textured" objects
    Resources::ResourceId dummyResId = "tex:system/ui_placeholder"NEBULA3_TEXTURE_EXTENSION;
    Ptr<ManagedTexture> managedTexture = ResourceManager::Instance()->CreateManagedResource(Texture::RTTI, dummyResId).downcast<ManagedTexture>();      
    this->texturePool.Add(Resources::ResourceId(""), managedTexture);
}

//------------------------------------------------------------------------------
/**
*/
void
CoreUIRendererBase::Discard()
{
	n_assert(this->IsValid());
    this->isValid = false;   
    this->sortedElements.Clear();
    this->texturePool.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
CoreUIRendererBase::LoadTexture(const Resources::ResourceId& resId)
{
    n_assert(!this->texturePool.Contains(resId));

    // build a full resource path
    String fullTexResId;
    fullTexResId.Format("tex:%s%s", resId.Value(), NEBULA3_TEXTURE_EXTENSION);
    Ptr<ManagedTexture> managedTexture = ResourceManager::Instance()->CreateManagedResource(Texture::RTTI, fullTexResId).downcast<ManagedTexture>();      

    // create new entry
    this->texturePool.Add(resId, managedTexture);
}

//------------------------------------------------------------------------------
/**
*/
void
CoreUIRendererBase::UnloadTexture(const Resources::ResourceId& resId)
{
    n_assert(this->texturePool.Contains(resId));
    ResourceManager::Instance()->DiscardManagedResource(this->texturePool[resId].cast<Resources::ManagedResource>());
    this->texturePool.Erase(resId);
}

//------------------------------------------------------------------------------
/**
*/
bool
CoreUIRendererBase::CheckTexturesLoaded() const
{
    IndexT i;
    for (i = 0; i < this->texturePool.Size(); i++)
    {
        if (this->texturePool.ValueAtIndex(i)->GetState() == Resource::Pending)
        {
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
CoreUIRendererBase::SetElements(const Array<CoreUI::CoreUIElement>& elements)
{
    n_assert(this->IsValid());
    this->sortedElements.Clear();
    if (elements.Size() > 0)
    {
        this->sortedElements.Reserve(elements.Size());
        this->sortedElements.BeginBulkAdd();
        IndexT i;
        for (i = 0; i < elements.Size(); i++)
        {
            // compute a key from the element's depth  and 
            // it's resource id, this packs elements with the
            // same depth and resource id next to each other
            // -- we use 12 bit for the depth and 20 bit for the resource id
            int key = (elements[i].GetDepth() << 20) | (int(elements[i].GetTextureResId().Value()) & 0xfffff);
            this->sortedElements.Add(key, elements[i]);
        }
        this->sortedElements.EndBulkAdd();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
CoreUIRendererBase::DrawElements(const Ptr<CoreGraphics::ShaderInstance>& shdInst)
{
    // IMPLEMENT IN SUBCLASS
}

} // namespace Base
