#pragma once
//------------------------------------------------------------------------------
/**
    A NetStream is a bitstream with a associated MessageId.
*/
#include "internalmultiplayer/base/netstreambase.h"
#include "extlibs/raknet/bitstream.h"

namespace RakNet
{
class RakNetStream : public Base::NetStreamBase
{
    __DeclareClass(RakNetStream);
public:
    /// constructor
    RakNetStream();
    ///destructor
    virtual ~RakNetStream();

    /// open the stream and create bitstream
    virtual bool Open();
    /// close the stream and destroy bitstream
    virtual void Close();
    /// can read
    virtual bool CanRead() const;
    /// can write
    virtual bool CanWrite() const;

    /// directly write to the stream
    virtual void Write(const void* ptr, Size numBytes);
    /// directly write BITs to the stream
    virtual void WriteBits(const unsigned char* ptr, uint numBits);
    
    /// directly read from the stream includes message id
    virtual Size Read(void* ptr, Size numBytes);
    /// directly read BITs from the message 
    virtual Size ReadBits(unsigned char* ptr, uint numBits);

    /// get bit stream
    BitStream* GetBitstream() const;
    /// get size of whole stream in bytes
    Stream::Size GetSize() const;
    /// get size of whole stream in bits
    Stream::Size GetSizeInBits() const;

    /// initialize from data, will copy data to bitstream->data
    void FromData(unsigned char* data, SizeT bits);

    /// print bits, needs char buffer with write
    Util::String PrintBitsAsString() const;

    /// seek to a bit index
    void SetReadIndex(IndexT bitIndex);
    /// get read bit pointer index
    IndexT GetReadIndex() const;   
    /// ignore bits
    void IgnoreBits(SizeT numBits);
    
private:
    BitStream* bitStream;
};
    
//------------------------------------------------------------------------------
/**
*/
inline BitStream* 
RakNetStream::GetBitstream() const
{
    n_assert(0 != this->bitStream);
    return this->bitStream;
}
}