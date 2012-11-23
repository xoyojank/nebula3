#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalMultiplayer::InternalPlayerBase
        
    (C) 2009 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "util/stringatom.h"
#include "internalmultiplayer/uniqueplayerid.h"

namespace InternalMultiplayer
{
    class InternalSession;
}

//------------------------------------------------------------------------------
namespace Base
{
class InternalPlayerBase : public Core::RefCounted
{
    __DeclareClass(InternalPlayerBase);
public:
    enum SignInState
    {
        NotSignedIn = 0,
        SignedInLocally,
        SignedInOnline,

        Unkown
    };
    /// constructor
    InternalPlayerBase();
    /// destructor
    virtual ~InternalPlayerBase();
    /// get PlayerIndex	TOOD: move to platform specific code, on win32 we have no local players
    IndexT GetLocalPlayerIndex() const;
    /// set PlayerIndex
    void SetLocalPlayerIndex(IndexT val);
    /// get SignInState	
    Base::InternalPlayerBase::SignInState GetSignInState() const;
    /// set SignInState
    void SetSignInState(Base::InternalPlayerBase::SignInState val);
    /// get PlayerName	
    const Util::StringAtom& GetPlayerName() const;
    /// set PlayerName
    void SetPlayerName(const Util::StringAtom& val);

    /// set player ready, overwrite in subclass
    void SetReady(bool val);
    /// is player ready, overwrite in subclass
    bool IsReady() const;

    /// get Id	
    const InternalMultiplayer::UniquePlayerId& GetUnqiueId() const;
    /// set Id
    void SetUniqueId(const InternalMultiplayer::UniquePlayerId& val);

    /// get IsLocal	
    bool IsLocal() const;
    /// set IsLocal
    void SetLocal(bool val);
    /// get IsHost	
    bool IsHost() const;
    /// set IsHost
    void SetHost(bool val);

    /// is player in session
    bool IsInSession() const;
    /// append player info to netstream
    void AppendPlayerInfoToStream(const Ptr<InternalMultiplayer::BitWriter>& writer);
    /// set player info from netstream
    void SetPlayerInfoFromStream(const Ptr<InternalMultiplayer::BitReader>& reader);   
    /// on join session 
    virtual void OnJoinSession(InternalMultiplayer::InternalSession* sessionPtr);
    /// on leave session
    virtual void OnLeaveSession();
    
protected:
    friend class InternalMultiplayer::InternalSession;
    /// get IsInPublicSlot	
    bool GetIsInPublicSlot() const;
    /// set IsInPublicSlot
    void SetIsInPublicSlot(bool val);

    IndexT playerIndex;
    SignInState signInState;
    bool isReady;
    bool isLocal;
    bool isHost;
    bool isInPublicSlot; // if false player is in pribate slot
    Util::StringAtom playerName;
    InternalMultiplayer::UniquePlayerId id;
    InternalMultiplayer::InternalSession* session; // back ptr
};

//------------------------------------------------------------------------------
/**
*/
inline IndexT 
InternalPlayerBase::GetLocalPlayerIndex() const
{
    return this->playerIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
InternalPlayerBase::SetLocalPlayerIndex(IndexT val)
{
    this->playerIndex = val;
}

//------------------------------------------------------------------------------
/**
*/
inline Base::InternalPlayerBase::SignInState 
InternalPlayerBase::GetSignInState() const
{
    return this->signInState;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
InternalPlayerBase::SetSignInState(Base::InternalPlayerBase::SignInState val)
{
    this->signInState = val;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
InternalPlayerBase::GetPlayerName() const
{
    return this->playerName;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
InternalPlayerBase::SetPlayerName(const Util::StringAtom& val)
{
    this->playerName = val;
}

//------------------------------------------------------------------------------
/**
*/
inline const InternalMultiplayer::UniquePlayerId& 
InternalPlayerBase::GetUnqiueId() const
{
    return this->id;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
InternalPlayerBase::SetUniqueId(const InternalMultiplayer::UniquePlayerId& val)
{
    this->id = val;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
InternalPlayerBase::IsLocal() const
{
    return this->isLocal;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
InternalPlayerBase::SetLocal(bool val)
{
    this->isLocal = val;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
InternalPlayerBase::IsInSession() const
{
    return this->session != 0;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
InternalPlayerBase::GetIsInPublicSlot() const
{
    return this->isInPublicSlot;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
InternalPlayerBase::SetIsInPublicSlot(bool val)
{
    this->isInPublicSlot = val;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
InternalPlayerBase::IsHost() const
{
    return this->isHost;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
InternalPlayerBase::SetHost(bool val)
{
    this->isHost = val;
}
} // namespace InternalMultiplayer
//------------------------------------------------------------------------------
