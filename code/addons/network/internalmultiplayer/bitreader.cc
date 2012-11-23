//------------------------------------------------------------------------------
//  bitreader.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalmultiplayer/bitreader.h"
#include "internalmultiplayer/timestamp.h"

namespace InternalMultiplayer
{
__ImplementClass(InternalMultiplayer::BitReader, 'IMBR', IO::BinaryReader);

using namespace Util;
using namespace System;
using namespace Math;
using namespace Base;

//------------------------------------------------------------------------------
/**
*/
BitReader::BitReader() : 
    packetId(PacketId::InvalidNebulaId),
    relativeTimeStamp(-1),
    hasTimeStamp(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
BitReader::~BitReader()
{
}

//------------------------------------------------------------------------------
/**
*/
bool 
BitReader::Open()
{
    if (IO::StreamReader::Open())
    {        
        // order must be the same as in writer, and timestamp must come first
        this->ReadTimeStamp();
        this->ReadPacketId();
        this->stream.cast<NetStreamBase>()->SetReliable( this->ReadBool() );
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
char
BitReader::ReadChar()
{
    unsigned char out;
    this->stream.cast<NetStreamBase>()->ReadBits(&out, sizeof(char)*8);
    return (char)out;
}

//------------------------------------------------------------------------------
/**
*/
unsigned char
BitReader::ReadUChar()
{
    unsigned char out;
    this->stream.cast<NetStreamBase>()->ReadBits(&out, sizeof(unsigned char)*8);
    return out;
}

//------------------------------------------------------------------------------
/**
*/
short
BitReader::ReadShort()
{
    short out;
    this->stream.cast<NetStreamBase>()->ReadBits((unsigned char*)&out, sizeof(short)*8);
    return out;
}

//------------------------------------------------------------------------------
/**
*/ 
unsigned short
BitReader::ReadUShort()
{
    unsigned short out;
    this->stream.cast<NetStreamBase>()->ReadBits((unsigned char*)&out, sizeof(unsigned short)*8);
    return out;
}

//------------------------------------------------------------------------------
/**
*/
int
BitReader::ReadInt()
{
    int out;
    this->stream.cast<NetStreamBase>()->ReadBits((unsigned char*)&out, sizeof(int)*8);
    return out;
}

//------------------------------------------------------------------------------
/**
*/
unsigned int
BitReader::ReadUInt()
{
    unsigned int out;
    this->stream.cast<NetStreamBase>()->ReadBits((unsigned char*)&out, sizeof(unsigned int)*8);
    return out;
}

//------------------------------------------------------------------------------
/**
*/
float
BitReader::ReadFloat()
{
    float out;
    this->stream.cast<NetStreamBase>()->ReadBits((unsigned char*)&out, sizeof(float)*8);
    return out;
}   

//------------------------------------------------------------------------------
/**
*/
float
BitReader::ReadFloatFromNormalizedUByte2()
{
    unsigned short compressedVal = this->ReadUShort();
    float result = ((float)compressedVal / 32767.5f) + 1.0f;
    return result;
}

//------------------------------------------------------------------------------
/**
*/
float
BitReader::ReadFloatFromUnsignedNormalizedUByte2()
{
    unsigned short compressedVal = this->ReadUShort();
    float result = (float)compressedVal / 65535.0f;
    return result;
}

//------------------------------------------------------------------------------
/**
*/
double
BitReader::ReadDouble()
{
    double out;
    this->stream.cast<NetStreamBase>()->ReadBits((unsigned char*)&out, sizeof(double)*8);
    return out;
}

//------------------------------------------------------------------------------
/**
*/
bool
BitReader::ReadBool()
{
    unsigned char out = 0;
    this->stream.cast<NetStreamBase>()->ReadBits(&out, 1);
    return out > 0x00;
}

//------------------------------------------------------------------------------
/**
*/
Util::String
BitReader::ReadString()
{
    // read size
    ushort size = this->ReadUShort();
    // now data
    char* out = (char*)Memory::Alloc(Memory::ScratchHeap, size+1);
    this->stream.cast<NetStreamBase>()->ReadBits((unsigned char*)out, size*8);
    out[size] = '\0';
    Util::String string((char*)out);
    return string;
}

//------------------------------------------------------------------------------
/**
*/
Util::Blob
BitReader::ReadBlob()
{
    // read size
    ushort size = this->ReadUShort();
    // now data
    Util::String string;
    void* out = Memory::Alloc(Memory::ScratchHeap, size);
    this->stream.cast<NetStreamBase>()->ReadBits((unsigned char*)out, size*8);
    return Util::Blob(out, size);
}

//------------------------------------------------------------------------------
/**
*/
Util::Guid
BitReader::ReadGuid()
{
    Util::Blob blob = this->ReadBlob();
    return Util::Guid::FromBinary((const unsigned char*)blob.GetPtr(), blob.Size());
}

//------------------------------------------------------------------------------
/**
*/
Math::float2 
BitReader::ReadFloat2()
{
    Math::float2 f;
    f.x() = this->ReadFloat();
    f.y() = this->ReadFloat();
    return f;
}

//------------------------------------------------------------------------------
/**
*/
Math::float4
BitReader::ReadFloat4()
{    
    Math::float4 f;
    f.x() = this->ReadFloat();
    f.y() = this->ReadFloat();
    f.z() = this->ReadFloat();
    f.w() = this->ReadFloat();
    return f;
}  

//------------------------------------------------------------------------------
/**
*/
Math::point 
BitReader::ReadPoint()
{
    Math::point f;
    f.x() = this->ReadFloat();
    f.y() = this->ReadFloat();
    f.z() = this->ReadFloat();
    return f;
}

//------------------------------------------------------------------------------
/**
*/
Math::vector 
BitReader::ReadVector()
{
    Math::vector f;
    f.x() = this->ReadFloat();
    f.y() = this->ReadFloat();
    f.z() = this->ReadFloat();
    return f;
}

//------------------------------------------------------------------------------
/**
*/
Math::matrix44
BitReader::ReadMatrix44()
{
    Math::matrix44 m;
    m.setrow0(this->ReadFloat4());
    m.setrow1(this->ReadFloat4());
    m.setrow2(this->ReadFloat4());
    m.setrow3(this->ReadFloat4());
    return m;
}

//------------------------------------------------------------------------------
/**
*/
bool 
BitReader::ReadBit()
{
    return this->ReadBool();
}

//------------------------------------------------------------------------------
/**
*/
IO::Stream::Size
BitReader::ReadRawData(void* ptr, SizeT numBytes)
{
    return this->stream.cast<NetStreamBase>()->Read(ptr, numBytes);
}

//------------------------------------------------------------------------------
/**
*/
IO::Stream::Size 
BitReader::ReadRawBitData( unsigned char* ptr, SizeT numBits )
{
    return this->stream.cast<NetStreamBase>()->ReadBits(ptr, numBits);
}

//------------------------------------------------------------------------------
/**
*/
void 
BitReader::SetStream( const Ptr<IO::Stream>& s )
{
    n_assert(s->IsA(NetStreamBase::RTTI));
    this->stream = s;
}

//------------------------------------------------------------------------------
/**
*/
void 
BitReader::ReadPacketId()
{
    this->ReadRawBitData((unsigned char*)&this->packetId, sizeof(PacketId::PacketIdType) * 8);
}

//------------------------------------------------------------------------------
/**
*/
void 
BitReader::ReadTimeStamp()
{   
    // first read flag if stream has timestamp
    this->hasTimeStamp = TimeStamp::HasRelativeTimeStamp(this);
    if (this->hasTimeStamp)
    {
        this->relativeTimeStamp = TimeStamp::ReadRelativeTimeStamp(this);
    }    
}

//------------------------------------------------------------------------------
/**
*/
void 
BitReader::SetReadIndex(IndexT readIndex)
{
    this->stream.cast<InternalMultiplayer::NetStream>()->SetReadIndex(readIndex);
}

//------------------------------------------------------------------------------
/**
*/
IndexT 
BitReader::GetReadIndex() const
{
    return this->stream.cast<InternalMultiplayer::NetStream>()->GetReadIndex();
}

//------------------------------------------------------------------------------
/**
*/
void 
BitReader::IgnoreBits(SizeT numBits)
{ 
    this->stream.cast<InternalMultiplayer::NetStream>()->IgnoreBits(numBits);
}

} // namespace InternalMultiplayer
