#ifndef __RAKNET_SOCKET_H
#define __RAKNET_SOCKET_H

#include "RakNetTypes.h"
#include "RakNetDefines.h"
// #include "SocketIncludes.h"

struct RakNetSocket
{
	RakNetSocket();
	~RakNetSocket();
	// SocketIncludes.h includes Windows.h, which messes up a lot of compiles
	// SOCKET s;
	unsigned int s;
	SocketDescriptor::SocketType socketType;
	unsigned int userConnectionSocketIndex;
	SystemAddress boundAddress;
#if defined (_WIN32) && defined(USE_WAIT_FOR_MULTIPLE_EVENTS)
	void* recvEvent;
#endif

};

#endif
