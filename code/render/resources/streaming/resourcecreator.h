#pragma once
//------------------------------------------------------------------------------
/*
    @class Resources::ResourceCreator

    A ResourceCreator allocates memory and sets up a Resource
    on a specific platform. (i.e. Win32Textures)

    (C) 2010 Radon Labs GmbH
*/
#include "core/refcounted.h"

//------------------------------------------------------------------------------
namespace Resources
{
class Resource;
class ResourceInfo;

class ResourceCreator : public Core::RefCounted
{
    __DeclareClass(ResourceCreator);
public:
    /// constructor
    ResourceCreator();
    /// destructor
    ~ResourceCreator();

    /// creates a new resources on a specific platform
    virtual Ptr<Resource> CreateResource(const ResourceInfo* resourceInfo);    
};
} // namespace Resource
//------------------------------------------------------------------------------