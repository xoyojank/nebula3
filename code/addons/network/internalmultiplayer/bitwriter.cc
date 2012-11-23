//------------------------------------------------------------------------------
//  bitwriter.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalmultiplayer/bitwriter.h"
#include "internalmultiplayer/timestamp.h"

namespace InternalMultiplayer
{
__ImplementClass(InternalMultiplayer::BitWriter, 'IMBW', IO::BinaryWriter);

using namespace Util;
using namespace System;
using namespace Math;
using namespace Base;

//------------------------------------------------------------------------------
/**
*/
BitWriter::BitWriter() : 
    packetId(PacketId::InvalidNebulaId),
    writeTimeStamp(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
BitWriter::~BitWriter()
{
}

//------------------------------------------------------------------------------
/**
*/
bool 
BitWriter::Open()
{
    n_assert(PacketId::InvalidNebulaId != this->packetId);

    if (IO::StreamWriter::Open())
    {     
        // timestamp must be written before anything else (needed for raknet implementation)
        this->WriteTimeStamp();
        this->WritePacketId();
        this->WriteBool( this->stream.cast<NetStreamBase>()->IsReliable() );
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
BitWriter::WriteChar(char c)
{
    this->stream.cast<NetStreamBase>()->WriteBits((const unsigned char*)&c, sizeof(char)*8);
}

//------------------------------------------------------------------------------
/**
*/
void
BitWriter::WriteUChar(unsigned char c)
{   
    this->stream.cast<NetStreamBase>()->WriteBits((const unsigned char*)&c, sizeof(unsigned char)*8);
}

//------------------------------------------------------------------------------
/**
*/
void
BitWriter::WriteShort(short s)
{
    this->byteOrder.ConvertInPlace<short>(s);
    this->stream.cast<NetStreamBase>()->WriteBits((const unsigned char*)&s, sizeof(short)*8);
}

//------------------------------------------------------------------------------
/**
*/
void
BitWriter::WriteUShort(unsigned short s)
{
    this->byteOrder.ConvertInPlace<unsigned short>(s);
    this->stream.cast<NetStreamBase>()->WriteBits((const unsigned char*)&s, sizeof(unsigned short)*8);
}

//------------------------------------------------------------------------------
/**
*/
void
BitWriter::WriteInt(int i)
{
    this->byteOrder.ConvertInPlace<int>(i);
    this->stream.cast<NetStreamBase>()->WriteBits((const unsigned char*)&i, sizeof(int)*8);
}

//------------------------------------------------------------------------------
/**
*/
void
BitWriter::WriteUInt(unsigned int i)
{    
    this->byteOrder.ConvertInPlace<unsigned int>(i);
    this->stream.cast<NetStreamBase>()->WriteBits((const unsigned char*)&i, sizeof(unsigned int)*8);
}

//------------------------------------------------------------------------------
/**
*/
void
BitWriter::WriteFloat(float f)
{
    this->byteOrder.ConvertInPlace<float>(f);
    this->stream.cast<NetStreamBase>()->WriteBits((const unsigned char*)&f, sizeof(float)*8);
}

//------------------------------------------------------------------------------
/**
*/
void
BitWriter::WriteFloatAsNormalizedUByte2(float f)
{
    n_assert(f >= -1.0f && f <= 1.0f);
    unsigned short compressedVal = (unsigned short)((f + 1.0f) * 32767.5f);   
    this->WriteUShort(compressedVal);
}

//------------------------------------------------------------------------------
/**
*/
void 
BitWriter::WriteFloatAsUnsignedNormalizedUByte2(float f)
{
    n_assert(f >= 0.0f && f <= 1.0f);
    unsigned short compressedVal = (unsigned short)(f * 65535.0f);   
    this->WriteUShort(compressedVal);
}

//------------------------------------------------------------------------------
/**
*/
void
BitWriter::WriteDouble(double d)
{
    this->byteOrder.ConvertInPlace<double>(d);
    this->stream.cast<NetStreamBase>()->WriteBits((const unsigned char*)&d, sizeof(double)*8);
}

//------------------------------------------------------------------------------
/**
*/
void
BitWriter::WriteBool(bool b)
{
    const unsigned char bT = b == true ? 0xFF : 0x00;
    const unsigned char* ptr = &bT;
    this->stream.cast<NetStreamBase>()->WriteBits(ptr, 1);
}

//------------------------------------------------------------------------------
/**
    NOTE: for strings, first the length will be written into a
    32-bit int, then the string contents without the 0-terminator.
*/
void
BitWriter::WriteString(const Util::String& s)
{
    const unsigned char* sT = (const unsigned char*)s.AsCharPtr();
    this->WriteUShort((ushort)s.Length());
    this->stream.cast<NetStreamBase>()->Write(sT, s.Length());
}

//------------------------------------------------------------------------------
/**
*/
void
BitWriter::WriteBlob(const Util::Blob& blob)
{
    const unsigned char* blobT = (const unsigned char*)blob.GetPtr();
    this->WriteUShort((ushort)blob.Size());
    this->stream.cast<NetStreamBase>()->Write(blobT, blob.Size());
}

//------------------------------------------------------------------------------
/**
*/
void
BitWriter::WriteGuid(const Util::Guid& guid)
{
    const unsigned char* ptr;
    SizeT size = guid.AsBinary(ptr);
    Util::Blob blob(ptr, size);
    this->WriteBlob(blob);
}
             
//------------------------------------------------------------------------------
/**
*/
void 
BitWriter::WriteFloat2(Math::float2 f)
{
    this->WriteFloat(f.x());
    this->WriteFloat(f.y());
}

//------------------------------------------------------------------------------
/**
*/
void
BitWriter::WriteFloat4(const float4& v)
{
    this->WriteFloat(v.x());
    this->WriteFloat(v.y());
    this->WriteFloat(v.z());
    this->WriteFloat(v.w());
}    

//------------------------------------------------------------------------------
/**
*/
void 
BitWriter::WritePoint(const Math::point& v)
{
    this->WriteFloat(v.x());
    this->WriteFloat(v.y());
    this->WriteFloat(v.z());
}

//------------------------------------------------------------------------------
/**
*/
void 
BitWriter::WriteVector(const Math::vector& v)
{
    this->WriteFloat(v.x());
    this->WriteFloat(v.y());
    this->WriteFloat(v.z());
}

//------------------------------------------------------------------------------
/**
*/
void
BitWriter::WriteMatrix44(const matrix44& m)
{
    this->WriteFloat4(m.getrow0());
    this->WriteFloat4(m.getrow1());
    this->WriteFloat4(m.getrow2());
    this->WriteFloat4(m.getrow3());
}

//------------------------------------------------------------------------------
/**
*/
void
BitWriter::WriteRawData(const void* ptr, SizeT numBytes)
{
    this->WriteRawBitData((const unsigned char*)ptr, numBytes * 8);
}

//------------------------------------------------------------------------------
/**
*/
void 
BitWriter::WriteBit( bool b )
{
    this->WriteBool(b);
}

//------------------------------------------------------------------------------
/**
*/
void 
BitWriter::WriteRawBitData( const unsigned char* ptr, SizeT numBits )
{
    this->stream.cast<NetStreamBase>()->WriteBits(ptr, numBits);
}

//------------------------------------------------------------------------------
/**
*/
void 
BitWriter::SetStream( const Ptr<IO::Stream>& s )
{
    n_assert(s->IsA(NetStreamBase::RTTI));
    this->stream = s;
}

//------------------------------------------------------------------------------
/**
*/
void 
BitWriter::WritePacketId()
{
    this->WriteRawBitData((const unsigned char*)&this->packetId, sizeof(PacketId::PacketIdType)*8);
}

//------------------------------------------------------------------------------
/**
*/
void 
BitWriter::WriteTimeStamp()
{
    if (this->writeTimeStamp)
    {        
        TimeStamp::WriteTimeStamp(this, this->timeStamp);
    }
    else
    {        
        TimeStamp::WriteNoTimeStampFlag(this);
    }    
}
} // namespace InternalMultiplayer
