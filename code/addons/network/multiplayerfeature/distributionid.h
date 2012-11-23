#pragma once

namespace MultiplayerFeature
{
class DistributionId
{

public:
    // used for writing id into stream with correct byte size
    typedef unsigned char DistributionIdType;

    enum DistributionIdCode
    {
        CreateObjectView,
        DestroyEntities,
        WorldSnapShot,
        ClientActions,
        
        NumDistributionIds,
        InvalidDistributionId
    };

};
} // RakNet