#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalMultiplayer::BitReader
    
    A friendly interface to read data  bitwise from a stream. Optionally the
    reader can use memory mapping for optimal read performance. Performs
    automatic byte order conversion if necessary.

    Works just with netstreams.

    The timestamp which is written by the bitwriter will be converted to a relative timestamp 
    between sender and receiver by the network system.

    (C) 2009 Radon Labs GmbH
*/
#include "io/binaryreader.h"
#include "system/byteorder.h"
#include "util/blob.h"
#include "util/guid.h"
#include "internalmultiplayer/packetid.h"
#include "timing/time.h"
#include "internalmultiplayer/netstream.h"

//------------------------------------------------------------------------------
namespace InternalMultiplayer
{
class BitReader : public IO::BinaryReader
{
    __DeclareClass(BitReader);
public:
    /// constructor
    BitReader();
    /// destructor
    virtual ~BitReader();

    /// begin reading from the stream
    virtual bool Open();

    /// set stream to write to
    void SetStream(const Ptr<IO::Stream>& s);
    /// get the packet id
    PacketId::PacketIdCode GetPacketId() const;
    /// get send duration in ticks
    Timing::Tick GetRelativeTimeStamp() const;
    /// has stream a send duration?
    bool HasRelativeTimeStamp() const;

    /// is the stream marked as reliable?
    bool IsStreamReliable() const;

    /// set the stream byte order (default is host byte order)
    void SetStreamByteOrder(System::ByteOrder::Type byteOrder);
    /// get the stream byte order
    System::ByteOrder::Type GetStreamByteOrder() const;

    /// read an 8-bit char from the stream
    char ReadChar();
    /// read an 8-bit unsigned character from the stream
    unsigned char ReadUChar();
    /// read a 16-bit short from the stream
    short ReadShort();
    /// read a 16-bit unsigned short from the stream
    unsigned short ReadUShort();
    /// read a 32-bit int from the stream
    int ReadInt();
    /// read a 32-bit unsigned int from the stream
    unsigned int ReadUInt();
    /// read a float value from the stream
    float ReadFloat();
    /// read a compressed float value from the stream, lossy and needed to be in the range of -1.0 and +1.0
    float ReadFloatFromNormalizedUByte2();
    /// read a compressed float value from the stream, lossy and needed to be in the range of 0.0 and +1.0
    float ReadFloatFromUnsignedNormalizedUByte2();
    /// read a double value from the stream
    double ReadDouble();
    /// read a bool value from the stream
    bool ReadBool();
    /// read a string from the stream
    Util::String ReadString();        
    /// read a float2 from the stream
    Math::float2 ReadFloat2();
    /// read a float4 from the stream
    Math::float4 ReadFloat4();
    /// read a point from the stream, (x,y,z,1.0)
    Math::point ReadPoint();
    /// read a vector from the stream, (x,y,z,0.0)
    Math::vector ReadVector();
    /// read a matrix44 from the stream
    Math::matrix44 ReadMatrix44();
    /// read a guid
    Util::Guid ReadGuid();
    /// read a blob of data
    Util::Blob ReadBlob();
    
    /// read a single bit
    bool ReadBit();
    /// read raw data
    IO::Stream::Size ReadRawData(void* ptr, SizeT numBytes);
    /// read raw bit data
    IO::Stream::Size ReadRawBitData(unsigned char* ptr, SizeT numBits); 
    /// ignore bits
    void IgnoreBits(SizeT numBits);

    /// seek to given bit index
    void SetReadIndex(IndexT readIndex);
    /// get read bit pointer index
    IndexT GetReadIndex() const;

private:
    /// read the packet id
    void ReadPacketId();
    /// read timestamp
    void ReadTimeStamp();

    System::ByteOrder byteOrder;
    InternalMultiplayer::PacketId::PacketIdCode packetId;
    Timing::Tick relativeTimeStamp;
    bool hasTimeStamp;
};


//------------------------------------------------------------------------------
/**
*/
inline bool 
BitReader::IsStreamReliable() const
{
    return this->stream.cast<Base::NetStreamBase>()->IsReliable();
}

//------------------------------------------------------------------------------
/**
*/
inline PacketId::PacketIdCode 
BitReader::GetPacketId() const
{
    return this->packetId;
}

//------------------------------------------------------------------------------
/**
*/
inline void
BitReader::SetStreamByteOrder(System::ByteOrder::Type order)
{
    this->byteOrder.SetToByteOrder(order);
}

//------------------------------------------------------------------------------
/**
*/
inline System::ByteOrder::Type
BitReader::GetStreamByteOrder() const
{
    return this->byteOrder.GetToByteOrder();
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Tick 
BitReader::GetRelativeTimeStamp() const
{
    n_assert(this->IsOpen());
    return this->relativeTimeStamp;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
BitReader::HasRelativeTimeStamp() const
{
    n_assert(this->IsOpen());
    return this->hasTimeStamp;
}
} // namespace InternalMultiplayer
//------------------------------------------------------------------------------
