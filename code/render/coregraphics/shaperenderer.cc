//------------------------------------------------------------------------------
//  shaperenderer.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/shaperenderer.h"

namespace CoreGraphics
{
#if (__WIN32__ || __XBOX360__)
__ImplementClass(CoreGraphics::ShapeRenderer, 'SHPR', Win360::D3D9ShapeRenderer);
#elif __WII__
__ImplementClass(CoreGraphics::ShapeRenderer, 'SHPR', Wii::WiiShapeRenderer);
#elif __PS3__
__ImplementClass(CoreGraphics::ShapeRenderer, 'SHPR', PS3::PS3ShapeRenderer);
#else
#error "ShapeRenderer class not implemented on this platform!"
#endif
__ImplementSingleton(CoreGraphics::ShapeRenderer);

//------------------------------------------------------------------------------
/**
*/
ShapeRenderer::ShapeRenderer()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
ShapeRenderer::~ShapeRenderer()
{
    __DestructSingleton;
}

} // namespace CoreGraphics
    
