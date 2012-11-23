//------------------------------------------------------------------------------
//  RakNetTimeStamp.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalmultiplayer/raknet/raknettimestamp.h"
#include "internalmultiplayer/packetid.h"
#include "internalmultiplayer/netstream.h"
#include "extlibs/raknet/raknettypes.h"

namespace RakNet
{

using namespace InternalMultiplayer;
//------------------------------------------------------------------------------
/**
*/
void 
RakNetTimeStamp::WriteTimeStamp(const Ptr<InternalMultiplayer::BitWriter>& writer, Timing::Tick time)
{
    // OccasionalPing have to be on in raknet for accurate timestamp sync
    // Message id for timestamp must come first before anything else
    MessageID timeStampId = ID_TIMESTAMP;
    writer->WriteRawBitData((const unsigned char*)&timeStampId, sizeof(PacketId::PacketIdType)*8);    
    writer->WriteRawBitData((const unsigned char*)&time, sizeof(Timing::Tick)*8);  
}

//------------------------------------------------------------------------------
/**
*/
void 
RakNetTimeStamp::WriteNoTimeStampFlag(const Ptr<InternalMultiplayer::BitWriter>& writer)
{
    // just write nothing
}

//------------------------------------------------------------------------------
/**
*/
bool 
RakNetTimeStamp::HasRelativeTimeStamp(const Ptr<InternalMultiplayer::BitReader>& reader)
{
    // Message id for timestamp must come first
    // check first byte without incrementing read ptr, so get directly access to bitstream data
    const Ptr<InternalMultiplayer::NetStream>& netstream = reader->GetStream().cast<InternalMultiplayer::NetStream>();
    BitStream* bitstream = netstream->GetBitstream();
    unsigned char* data = bitstream->GetData(); 

    return (data[0] == ID_TIMESTAMP);
}

//------------------------------------------------------------------------------
/**
*/
Timing::Tick 
RakNetTimeStamp::ReadRelativeTimeStamp(const Ptr<InternalMultiplayer::BitReader>& reader)
{
    // check first byte, and consume it
    MessageID idCode = reader->ReadChar();
    n_assert(idCode == ID_TIMESTAMP);

    Timing::Tick timeStamp;
    return reader->ReadRawBitData((unsigned char*)&timeStamp, sizeof(Timing::Tick) * 8);
}
} // end RakNet