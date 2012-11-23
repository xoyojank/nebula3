#pragma once
//------------------------------------------------------------------------------
/**
    @class Lighting::LightServer
  
    The light server collects all lights contributing to the scene and
    controls the realtime lighting process.

    (C) 2007 Radon Labs GmbH
*/
#if (__XBOX360__)
#include "lighting/lightprepass/lightprepassserver.h"
namespace Lighting
{
    class LightServer : public Lighting::LightPrePassServer
    {
        __DeclareClass(LightServer);
        __DeclareSingleton(LightServer);
    public:
        /// constructor
        LightServer();
        /// destructor
        virtual ~LightServer();
    };
} // namespace Lighting
#elif __WII__
#include "lighting/wii/wiilightserver.h"
namespace Lighting
{
class LightServer : public Wii::WiiLightServer
{
    __DeclareClass(LightServer);
    __DeclareSingleton(LightServer);
public:
    /// constructor
    LightServer();
    /// destructor
    virtual ~LightServer();
};
} // namespace Lighting
#elif (__WIN32__ || __PS3__)
#include "lighting/lightprepass/lightprepassserver.h"
namespace Lighting
{
class LightServer : public Lighting::LightPrePassServer
{
    __DeclareClass(LightServer);
    __DeclareSingleton(LightServer);
public:
    /// constructor
    LightServer();
    /// destructor
    virtual ~LightServer();
};
} // namespace Lighting
#else
#error "LightServer class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
