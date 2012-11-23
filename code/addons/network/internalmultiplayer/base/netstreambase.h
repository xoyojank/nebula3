#pragma once
//------------------------------------------------------------------------------
/**
    A NetStream is a bitstream with a associated MessageId.
*/
#include "foundation/io/stream.h"
#include "messaging/message.h"
#include "internalmultiplayer/packetid.h"

namespace Base
{
class NetStreamBase : public IO::Stream
{
    __DeclareClass(NetStreamBase);
public:
    /// constructor
    NetStreamBase();
    ///destructor
    virtual ~NetStreamBase();

    /// directly write BITs to the stream
    virtual void WriteBits(const unsigned char* ptr, uint numBits);

    /// directly read BITs from the message 
    virtual Size ReadBits(unsigned char* ptr, uint numBits);

    /// ignore bits
    void IgnoreBits(SizeT numBits);

    /// seek to a bit index
    void SetReadIndex(IndexT bitIndex);  
    /// get bit index
    IndexT GetReadIndex() const;

    /// set stream as reliable
    void SetReliable(bool b);
    /// get reliability of stream
    bool IsReliable() const;

protected:
    bool isReliable;
};

//------------------------------------------------------------------------------
/**
*/
inline void 
NetStreamBase::SetReliable( bool b )
{
    this->isReliable = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
NetStreamBase::IsReliable() const
{
    return this->isReliable;
}
} 