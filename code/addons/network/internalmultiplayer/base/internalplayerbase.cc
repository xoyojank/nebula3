//------------------------------------------------------------------------------
//  internalplayerbase.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalmultiplayer/base/internalplayerbase.h"
#include "internalmultiplayer/internalsession.h"

namespace Base
{
__ImplementClass(Base::InternalPlayerBase, 'IPLB', Core::RefCounted);

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
InternalPlayerBase::InternalPlayerBase():
    playerIndex(InvalidIndex),
    signInState(Unkown),
    isReady(false),
    isLocal(true),
    isHost(false),
    isInPublicSlot(true),
    session(0)
{
}

//------------------------------------------------------------------------------
/**
*/
InternalPlayerBase::~InternalPlayerBase()
{
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalPlayerBase::OnJoinSession(InternalMultiplayer::InternalSession* sessionPtr)
{
    n_assert(sessionPtr != 0);
    n_assert(this->session == 0);
    this->session = sessionPtr;
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalPlayerBase::OnLeaveSession()
{
    n_assert(this->session != 0);
    this->session = 0;
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalPlayerBase::SetReady(bool val)
{
    this->isReady = val;
}

//------------------------------------------------------------------------------
/**
*/
bool 
InternalPlayerBase::IsReady() const
{  
    return this->isReady;
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalPlayerBase::AppendPlayerInfoToStream(const Ptr<InternalMultiplayer::BitWriter>& writer)
{
    // first id
    this->id.Encode(writer);
    // second name
    writer->WriteString(this->playerName.Value());
    // third ready state
    writer->WriteBool(this->IsReady());
    // is this player the host
    writer->WriteBool(this->IsHost());
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalPlayerBase::SetPlayerInfoFromStream(const Ptr<InternalMultiplayer::BitReader>& reader)
{
    // id 
    this->id.Decode(reader);
    // player name
    this->playerName = reader->ReadString();    
    // ready state
    this->SetReady(reader->ReadBool());
    // is this player the host
    this->SetHost(reader->ReadBool());
}
} // namespace InternalMultiplayer
