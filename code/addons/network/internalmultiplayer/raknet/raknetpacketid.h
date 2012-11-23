#pragma once
#include "extlibs/raknet/messageidentifiers.h"

namespace RakNet
{
#define PACKET_DATA_BYTEOFFSET  1

class RakNetPackedId
{

public:
    // used for writing id into stream with correct byte size
    typedef unsigned char PacketIdType;

    enum PacketIdCode
    {
        NebulaPlayerinfo = ID_USER_PACKET_ENUM,        
        NebulaStartGame,
        NebulaClientStartgameFinished,
        NebulaAllStartgameFinished,
        NebulaEndgame,  
        NebulaPlayerReadynessChanged,    
        NebulaMessage,
        NebulaMigrateHost,
        NebulaNewHost,
        
        NumNebulaPacketIds,
        InvalidNebulaId
    };

};
} // RakNet