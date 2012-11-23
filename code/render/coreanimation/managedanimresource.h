#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreAnimation::ManagedAnimation
    
    Managed wrapper for AnimResource class.
    
    (C) 2008 Radon Labs GmbH
*/
#include "resources/managedresource.h"
#include "coreanimation/animresource.h"

//------------------------------------------------------------------------------
namespace CoreAnimation
{
class ManagedAnimResource : public Resources::ManagedResource
{
    __DeclareClass(ManagedAnimResource);
public:
    /// get contained anim resource
    const Ptr<AnimResource>& GetAnimResource() const;
};

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<AnimResource>&
ManagedAnimResource::GetAnimResource() const
{
    return this->GetLoadedResource().downcast<AnimResource>();
}

} // namespace CoreAnimation
//------------------------------------------------------------------------------
    