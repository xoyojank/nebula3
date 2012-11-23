#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::TransformDevice
    
    Manages global transform matrices and their combinations. Input transforms 
    are the view transform (transforms from world to view space),
    the projection transform (describes the projection from view space
    into projection space (pre-div-z)) and the current model matrix
    (transforms from model to world space). From these input transforms,
    the TransformDevice computes all useful combinations and
    inverted versions.

    (C) 2007 Radon Labs GmbH
*/
#if (__WIN32__ || __XBOX360__)
#include "coregraphics/win360/d3d9transformdevice.h"
namespace CoreGraphics
{
class TransformDevice : public Win360::D3D9TransformDevice
{
    __DeclareClass(TransformDevice);
    __DeclareSingleton(TransformDevice);
public:
    /// constructor
    TransformDevice();
    /// destructor
    virtual ~TransformDevice();
};
}
#elif __WII__
#include "coregraphics/wii/wiitransformdevice.h"
namespace CoreGraphics
{
class TransformDevice : public Wii::WiiTransformDevice
{
    __DeclareClass(TransformDevice);
    __DeclareSingleton(TransformDevice);
public:
    /// constructor
    TransformDevice();
    /// destructor
    virtual ~TransformDevice();
};
}
#elif __PS3__
#include "coregraphics/ps3/ps3transformdevice.h"
namespace CoreGraphics
{
class TransformDevice : public PS3::PS3TransformDevice
{
    __DeclareClass(TransformDevice);
    __DeclareSingleton(TransformDevice);
public:
    /// constructor
    TransformDevice();
    /// destructor
    virtual ~TransformDevice();
};
}
#else
#error "TransformDevice class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------

