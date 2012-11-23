//------------------------------------------------------------------------------
//  uirenderer.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "ui/rt/coreuirenderer.h"

namespace CoreUI
{
#if (__WIN32__)
__ImplementClass(CoreUI::CoreUIRenderer, 'CUIR', Win32::Win360CoreUIRenderer);
#elif  (__XBOX360__)
__ImplementClass(CoreUI::CoreUIRenderer, 'CUIR', Win32::Win360CoreUIRenderer);
#elif  (__PS3__)
    __ImplementClass(CoreUI::CoreUIRenderer, 'CUIR', PS3::PS3CoreUIRenderer);
#else
#error "CoreUIRenderer class not implemented on this platform!"
#endif
__ImplementSingleton(CoreUI::CoreUIRenderer);

//------------------------------------------------------------------------------
/**
*/
CoreUIRenderer::CoreUIRenderer()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
CoreUIRenderer::~CoreUIRenderer()
{
    __DestructSingleton;
}

} // namespace CoreUI
    
