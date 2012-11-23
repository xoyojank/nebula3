//------------------------------------------------------------------------------
//  suiserver.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "sui/rt/suiserver.h"

namespace SUI
{
#if (__WIN32__||__XBOX360__)
__ImplementClass(SUI::SUIServer, 'SUIS', Base::SUIServerBase);
#elif __WII__
__ImplementClass(SUI::SUIServer, 'SUIS', Wii::WiiSUIServer);
#else
#error "SUI::SUIServer not implemented on this platform"
#endif
__ImplementSingleton(SUI::SUIServer);

//------------------------------------------------------------------------------
/**
*/
SUIServer::SUIServer()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
SUIServer::~SUIServer()
{
    __DestructSingleton;
}

} // namespace SUI