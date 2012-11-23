#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::MultipleRenderTarget
    
    A multiple render targets wraps up to 4 rendertargets into a C++ object. The special default render target represents the 
    backbuffer and default depth/stencil surface.

    (C) 2007 Radon Labs GmbH
*/
#if (__WIN32__ || __XBOX360__)
#include "coregraphics/base/multiplerendertargetbase.h"
namespace CoreGraphics
{
class MultipleRenderTarget : public Base::MultipleRenderTargetBase
{
    __DeclareClass(MultipleRenderTarget);
};
}
#elif __WII__
#include "coregraphics/base/multiplerendertargetbase.h"
namespace CoreGraphics
{
class MultipleRenderTarget : public Base::MultipleRenderTargetBase
{
    __DeclareClass(MultipleRenderTarget);
};
}
#elif __PS3__
#include "coregraphics/base/multiplerendertargetbase.h"
namespace CoreGraphics
{
class MultipleRenderTarget : public Base::MultipleRenderTargetBase
{
    __DeclareClass(MultipleRenderTarget);
};
}
#else
#error "MultipleRenderTarget class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
