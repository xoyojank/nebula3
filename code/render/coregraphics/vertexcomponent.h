#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::VertexComponent
  
    Describes a single vertex component in a vertex layout description.
    
    (C) 2006 Radon Labs GmbH
*/    
#if (__WIN32__ || __XBOX360__ || __WII__)
#include "coregraphics/base/vertexcomponentbase.h"
namespace CoreGraphics
{
class VertexComponent : public Base::VertexComponentBase
{
public:
    /// default constructor
    VertexComponent() { };
    /// constructor
    VertexComponent(SemanticName semName_, IndexT semIndex_, Format format_, IndexT streamIndex_=0) : VertexComponentBase(semName_, semIndex_, format_, streamIndex_) { };
};
}
#elif __PS3__
#include "coregraphics/ps3/ps3vertexcomponent.h"
namespace CoreGraphics
{
class VertexComponent : public PS3::PS3VertexComponent
{
public:
    /// default constructor
    VertexComponent() { };
    /// constructor
    VertexComponent(SemanticName semName_, IndexT semIndex_, Format format_, IndexT streamIndex_=0) : PS3VertexComponent(semName_, semIndex_, format_, streamIndex_) { };
};
}
#else
#error "CoreGraphics::VertexComponent not implemented on this platform"
#endif
