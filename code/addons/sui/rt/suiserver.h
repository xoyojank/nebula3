#pragma once
//------------------------------------------------------------------------------
/**
    @class SUI::SUIServer

    The central server object of the SUI system, running in the render-thread.

    (C) 2009 Radon Labs GmbH
*/
#if (__WIN32__||__XBOX360__)
#include "sui/base/suiserverbase.h"
namespace SUI
{
class SUIServer : public Base::SUIServerBase
{
    __DeclareClass(SUIServer);
    __DeclareSingleton(SUIServer);
public:
    /// constructor
    SUIServer();
    /// destructor
    virtual ~SUIServer();    
};
}
#elif __WII__
#include "sui/wii/wiisuiserver.h"
namespace SUI
{
class SUIServer : public Wii::WiiSUIServer
{
    __DeclareClass(SUIServer);
    __DeclareSingleton(SUIServer);
public:
    /// constructor
    SUIServer();
    /// destructor
    virtual ~SUIServer();    
};
}
#else
#error "SUI::SUIServer not implemented on this platform"
#endif