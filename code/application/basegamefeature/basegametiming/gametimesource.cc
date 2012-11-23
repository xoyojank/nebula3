//------------------------------------------------------------------------------
//  gametimesource.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "basegametiming/gametimesource.h"

namespace BaseGameFeature
{
__ImplementClass(GameTimeSource, 'TSRC', TimeSource);
__ImplementSingleton(GameTimeSource);

//------------------------------------------------------------------------------
/**
*/
GameTimeSource::GameTimeSource()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
GameTimeSource::~GameTimeSource()
{
    __DestructSingleton;
}

}; // namespace Timing
