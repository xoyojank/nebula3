#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalMultiplayer::RakNetUniquePlayerId

    A UniquePlayerId identifies each player over the whole network.

(C) 2009 Radon Labs GmbH
*/    
#include "internalmultiplayer/base/uniqueplayeridbase.h"
#include "extlibs/raknet/raknettypes.h"

//------------------------------------------------------------------------------
namespace RakNet
{
class RakNetUniquePlayerId : public Base::UniquePlayerIdBase
{
public:
    /// constructor
    RakNetUniquePlayerId();
    /// constructor
    RakNetUniquePlayerId(const RakNetGUID& rakNetId);
    /// destructor
    virtual ~RakNetUniquePlayerId();
    /// equality operator
    friend bool operator==(const RakNetUniquePlayerId& a, const RakNetUniquePlayerId& b);
    /// unequality operator
    friend bool operator!=(const RakNetUniquePlayerId& a, const RakNetUniquePlayerId& b);
    /// less operator
    friend bool operator<(const RakNetUniquePlayerId& a, const RakNetUniquePlayerId& b);
    /// greater operator
    friend bool operator>(const RakNetUniquePlayerId& a, const RakNetUniquePlayerId& b);
    /// get Guid	
    const RakNetGUID& GetRaknetGuid() const;
    /// set Guid
    void SetRaknetGuid(const RakNetGUID& val);
    /// encode id into a stream
    virtual void Encode(const Ptr<InternalMultiplayer::BitWriter>& writer) const;
    /// decode id from a stream
    virtual void Decode(const Ptr<InternalMultiplayer::BitReader>& reader);
    /// convert to blob
    Util::Blob AsBlob() const;

protected:
    RakNetGUID  guid;
};
//------------------------------------------------------------------------------
/**
*/
inline const RakNetGUID& 
RakNetUniquePlayerId::GetRaknetGuid() const
{
    return this->guid;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
RakNetUniquePlayerId::SetRaknetGuid(const RakNetGUID& val)
{
    this->guid = val;
}
} // namespace InternalMultiplayer
//-------------------