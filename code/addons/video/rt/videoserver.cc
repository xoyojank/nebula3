//------------------------------------------------------------------------------
//  videoserver.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "video/rt/videoserver.h"

namespace Video
{
#if __WIN32__
__ImplementClass(Video::VideoServer, 'VIDS', Bink::BinkVideoServer);
#elif __XBOX360__
__ImplementClass(Video::VideoServer, 'VIDS', Xbox360::Xbox360VideoServer);
#elif __WII__
__ImplementClass(Video::VideoServer, 'VIDS', Wii::WiiVideoServer);
#elif __PS3__
__ImplementClass(Video::VideoServer, 'VIDS', Base::VideoServerBase);
#else
#error "InputServer class not implemented on this platform!"
#endif
__ImplementSingleton(Video::VideoServer);

//------------------------------------------------------------------------------
/**
*/
VideoServer::VideoServer()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
VideoServer::~VideoServer()
{
    __DestructSingleton;
}

} // namespace Video
