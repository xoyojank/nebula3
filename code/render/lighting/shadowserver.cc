//------------------------------------------------------------------------------
//  shadowserver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "lighting/shadowserver.h"

namespace Lighting
{
#if (__WIN32__ || __XBOX360__)
__ImplementClass(Lighting::ShadowServer, 'SDSV', Lighting::SM30ShadowServer);
#elif __WII__
__ImplementClass(Lighting::ShadowServer, 'SDSV', Wii::WiiShadowServer);
#elif __PS3__
__ImplementClass(Lighting::ShadowServer, 'SDSV', Lighting::ShadowServerBase);
#else
#error "ShadowServer class not implemented on this platform!"
#endif
__ImplementSingleton(Lighting::ShadowServer);

//------------------------------------------------------------------------------
/**
*/
ShadowServer::ShadowServer()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
ShadowServer::~ShadowServer()
{
    __DestructSingleton;
}

} // namespace Lighting
