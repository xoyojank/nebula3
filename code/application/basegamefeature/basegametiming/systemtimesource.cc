//--------------------------------------------------------------------
//  systemtimesource.cc
//  (C) 2007 Radon Labs GmbH
//--------------------------------------------------------------------
#include "stdneb.h"
#include "basegametiming/systemtimesource.h"

namespace BaseGameFeature
{
__ImplementClass(SystemTimeSource, 'TSTS', TimeSource);
__ImplementSingleton(SystemTimeSource);
//--------------------------------------------------------------------
/**
*/
SystemTimeSource::SystemTimeSource()
{
    __ConstructSingleton;
}

//--------------------------------------------------------------------
/**
*/
SystemTimeSource::~SystemTimeSource()
{
    __DestructSingleton;
}

}; // namespace Timing