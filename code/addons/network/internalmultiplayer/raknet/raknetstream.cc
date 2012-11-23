//------------------------------------------------------------------------------
//  raknetstream.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalmultiplayer/raknet/raknetstream.h"

namespace RakNet
{
__ImplementClass(RakNet::RakNetStream, 'RKST', Base::NetStreamBase);

using namespace IO;
//------------------------------------------------------------------------------
/**
*/
RakNetStream::RakNetStream():
    bitStream(0)    
{
    this->bitStream = n_new(RakNet::BitStream);
}

//------------------------------------------------------------------------------
/**
*/
RakNetStream::~RakNetStream()
{  
    n_delete(this->bitStream);
}

//------------------------------------------------------------------------------
/**
*/
bool 
RakNetStream::Open()
{
    n_assert(this->bitStream != 0);
    return Base::NetStreamBase::Open();
}

//------------------------------------------------------------------------------
/**
*/
void 
RakNetStream::Close()
{
    n_assert(this->bitStream != 0);    
    this->bitStream->ResetReadPointer();
    Stream::Close();
}

//------------------------------------------------------------------------------
/**
*/
bool 
RakNetStream::CanRead() const
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
RakNetStream::CanWrite() const
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
RakNetStream::Write(const void* ptr, Size numBytes)
{
    n_assert(this->IsOpen());

    this->bitStream->Write((char*)ptr, numBytes);
}

//------------------------------------------------------------------------------
/**
*/
Stream::Size
RakNetStream::Read(void* ptr, Size numBytes)
{
    n_assert(this->IsOpen());
    if (!this->bitStream->Read((char*)ptr, numBytes))
    {
        n_error("ATTENTION: RakNetStream::ReadBits() error while reading stream!\n");
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
Stream::Size 
RakNetStream::GetSize() const
{
    return this->bitStream->GetNumberOfBytesUsed();
}

//------------------------------------------------------------------------------
/**
*/
void 
RakNetStream::WriteBits( const unsigned char* ptr, uint numBits )
{
    this->bitStream->WriteBits((const unsigned char*)ptr, numBits);
}

//------------------------------------------------------------------------------
/**
*/
Stream::Size 
RakNetStream::ReadBits( unsigned char* ptr, uint numBits )
{    
    n_assert(ptr);
    if (!this->bitStream->ReadBits(ptr, numBits))
    {
        n_error("ATTENTION: RakNetStream::ReadBits() error while reading stream!\n");
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void 
RakNetStream::FromData(unsigned char* data, SizeT bits)
{
    // copy over data
    this->bitStream->WriteBits(data, bits, false);
}

//------------------------------------------------------------------------------
/**
*/
Stream::Size 
RakNetStream::GetSizeInBits() const
{
    return this->bitStream->GetNumberOfBitsUsed();
}

//------------------------------------------------------------------------------
/**
*/
Util::String 
RakNetStream::PrintBitsAsString() const
{       
    char* outBuffer = (char*)Memory::Alloc(Memory::ScratchHeap, this->GetSizeInBits() * 2);
    this->bitStream->PrintBits(outBuffer);
    Util::String result(outBuffer);
    Memory::Free(Memory::ScratchHeap, outBuffer);
    return result;
}

//------------------------------------------------------------------------------
/**
*/
void 
RakNetStream::SetReadIndex(IndexT bitIndex)
{
    this->bitStream->SetReadOffset(bitIndex);
}

//------------------------------------------------------------------------------
/**
*/
IndexT 
RakNetStream::GetReadIndex() const
{
    return this->bitStream->GetReadOffset();
}

//------------------------------------------------------------------------------
/**
*/
void 
RakNetStream::IgnoreBits(SizeT numBits)
{
    this->bitStream->IgnoreBits(numBits);
}
}