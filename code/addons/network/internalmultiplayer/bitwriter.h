#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalMultiplayer::BitWriter
    
    A friendly interface for writing data bitwise to a stream. Optionally
    the writer can use memory mapping for optimal write performance.

    The PacketId must be set correctly. If you don't need any timestamp 
    SetWriteTimeStamp to false to save 4 bytes. 
    
    Works just with netstreams!

    If any send duration is needed, just save your current time with 
    SetWriteTimeStamp. It is automatically converted to a relative timestamp
    on client side.
    
    (C) 2009 Radon Labs GmbH
*/
#include "io/binarywriter.h"
#include "util/guid.h"
#include "util/blob.h"
#include "system/byteorder.h"
#include "internalmultiplayer/packetid.h"
#include "timing/time.h"
#include "internalmultiplayer/base/netstreambase.h"

//------------------------------------------------------------------------------
namespace InternalMultiplayer
{
class BitWriter : public IO::BinaryWriter
{
    __DeclareClass(BitWriter);
public:
    /// constructor
    BitWriter();
    /// destructor
    virtual ~BitWriter();

    /// begin reading from the stream
    virtual bool Open();

    /// set stream to write to
    void SetStream(const Ptr<IO::Stream>& s);
    /// set the packet if
    void SetPacketId(PacketId::PacketIdCode id);
    /// set use time stamp, default is yes, set to false if your stream doesn't need a timestamp (will save 4 bytes)
    void SetWriteTimeStamp(Timing::Tick time);

    /// is the stream declared as reliable?
    bool IsStreamReliable() const;
    /// overwrite the stream reliability
    void SetStreamReliability(bool b);

    /// set the stream byte order (default is host byte order)
    void SetStreamByteOrder(System::ByteOrder::Type byteOrder);
    /// get the stream byte order
    System::ByteOrder::Type GetStreamByteOrder() const;

    /// write an 8-bit char to the stream
    void WriteChar(char c);
    /// write an 8-bit unsigned char to the stream
    void WriteUChar(unsigned char c);
    /// write an 16-bit short to the stream
    void WriteShort(short s);
    /// write an 16-bit unsigned short to the stream
    void WriteUShort(unsigned short s);
    /// write an 32-bit int to the stream
    void WriteInt(int i);
    /// write an 32-bit unsigned int to the stream
    void WriteUInt(unsigned int i);
    /// write a float value to the stream
    void WriteFloat(float f);
    /// write a compressed float value to the stream, lossy and needed to be in the range of -1.0 and +1.0
    void WriteFloatAsNormalizedUByte2(float f);
    /// write a compressed float value to the stream, lossy and needed to be in the range of 0.0 and +1.0
    void WriteFloatAsUnsignedNormalizedUByte2(float f);
    /// write a double value to the stream
    void WriteDouble(double d);
    /// write a boolean value to the stream
    void WriteBool(bool b);
    /// write a string to the stream
    void WriteString(const Util::String& s);
    /// write a float value to the stream    
    void WriteFloat2(Math::float2 f);
    /// write a float4 to the stream
    void WriteFloat4(const Math::float4& v);
    /// write a float4 to the stream
    void WritePoint(const Math::point& v);
    /// write a float4 to the stream
    void WriteVector(const Math::vector& v);
    /// write a matrix44 to the stream
    void WriteMatrix44(const Math::matrix44& m);
    /// write a guid
    void WriteGuid(const Util::Guid& guid);
    /// write a blob of data
    void WriteBlob(const Util::Blob& blob);

    /// write a single bit
    void WriteBit(bool b);
    /// write raw data
    void WriteRawData(const void* ptr, SizeT numBytes);
    /// write raw data bitwise
    void WriteRawBitData(const unsigned char* ptr, SizeT numBits);

private:
    /// write the packet id
    void WritePacketId();
    /// write timestamp
    void WriteTimeStamp();

    System::ByteOrder byteOrder;    
    InternalMultiplayer::PacketId::PacketIdCode packetId;
    bool writeTimeStamp;
    Timing::Tick timeStamp;
};

//------------------------------------------------------------------------------
/**
*/
inline bool 
BitWriter::IsStreamReliable() const
{
    return this->stream.cast<Base::NetStreamBase>()->IsReliable();
}


//------------------------------------------------------------------------------
/**
*/
inline void 
BitWriter::SetStreamReliability(bool b)
{
    return this->stream.cast<Base::NetStreamBase>()->SetReliable(b);
}

//------------------------------------------------------------------------------
/**
*/
inline void 
BitWriter::SetPacketId( PacketId::PacketIdCode id )
{
    this->packetId = id;
}

//------------------------------------------------------------------------------
/**
*/
inline void
BitWriter::SetStreamByteOrder(System::ByteOrder::Type order)
{
    this->byteOrder.SetToByteOrder(order);
}

//------------------------------------------------------------------------------
/**
*/
inline System::ByteOrder::Type
BitWriter::GetStreamByteOrder() const
{
    return this->byteOrder.GetToByteOrder();
}

//------------------------------------------------------------------------------
/**
*/
inline void 
BitWriter::SetWriteTimeStamp(Timing::Tick time)
{
    this->writeTimeStamp = true;
    this->timeStamp = time;
}
} // namespace InternalMultiplayer
//------------------------------------------------------------------------------
