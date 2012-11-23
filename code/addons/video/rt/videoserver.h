#pragma once
//------------------------------------------------------------------------------
/**
    @class Video::VideoServer
    
    The VideoServer is the central object of the Video subsystem. 

    (C) 2009 Radon Labs GmbH
*/
#include "core/singleton.h"
#if __WIN32__
#include "video/bink/binkvideoserver.h"
namespace Video
{
class VideoServer : public Bink::BinkVideoServer
{
    __DeclareClass(VideoServer);
    __DeclareSingleton(VideoServer);
public:
    /// constructor
    VideoServer();
    /// destructor
    virtual ~VideoServer();
};
} // namespace Video
#elif __XBOX360__
#include "video/xbox360/xbox360videoserver.h"
namespace Video
{
class VideoServer : public Xbox360::Xbox360VideoServer
{
    __DeclareClass(VideoServer);
    __DeclareSingleton(VideoServer);
public:
    /// constructor
    VideoServer();
    /// destructor
    virtual ~VideoServer();
};
} // namespace Video
#elif __WII__
#include "video/wii/wiivideoserver.h"
namespace Video
{
class VideoServer : public Wii::WiiVideoServer
{
    __DeclareClass(VideoServer);
    __DeclareSingleton(VideoServer);
public:
    /// constructor
    VideoServer();
    /// destructor
    virtual ~VideoServer();
};
} // namespace Video
#elif __PS3__
#include "video/base/videoserverbase.h"
namespace Video
{
class VideoServer : public Base::VideoServerBase
{
    __DeclareClass(VideoServer);
    __DeclareSingleton(VideoServer);
public:
    /// constructor
    VideoServer();
    /// destructor
    virtual ~VideoServer();
};
} // namespace Video
#else
#error "InputServer class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------