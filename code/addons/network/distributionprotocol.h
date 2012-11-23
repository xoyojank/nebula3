#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "multiplayerfeature/objectview.h"
#include "multiplayerprotocol.h"

//------------------------------------------------------------------------------
namespace MultiplayerFeature
{
//------------------------------------------------------------------------------
class CreateObjectView : public Messaging::Message
{
    __DeclareClass(CreateObjectView);
    __DeclareMsgId;
public:
    CreateObjectView() 
    { };
public:
    void SetObjectViewId(const ObjectView::ObjectViewId& val)
    {
        n_assert(!this->handled);
        this->objectviewid = val;
    };
    const ObjectView::ObjectViewId& GetObjectViewId() const
    {
        return this->objectviewid;
    };
private:
    ObjectView::ObjectViewId objectviewid;
};
//------------------------------------------------------------------------------
class PlayerMoveDirection : public Multiplayer::NetworkEventBase
{
    __DeclareClass(PlayerMoveDirection);
    __DeclareMsgId;
public:
    PlayerMoveDirection() 
    { };
public:
    void SetDirection(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->direction = val;
    };
    const Math::float4& GetDirection() const
    {
        return this->direction;
    };
private:
    Math::float4 direction;
public:
    void Encode(const Ptr<IO::BinaryWriter>& writer)
    {
        writer->WriteFloat4(this->GetDirection());
        Multiplayer::NetworkEventBase::Encode(writer);
    };
public:
    void Decode(const Ptr<IO::BinaryReader>& reader)
    {
        this->SetDirection(reader->ReadFloat4());
        Multiplayer::NetworkEventBase::Decode(reader);
    };
};
//------------------------------------------------------------------------------
class PlayerMoveStop : public Multiplayer::NetworkEventBase
{
    __DeclareClass(PlayerMoveStop);
    __DeclareMsgId;
public:
    PlayerMoveStop() 
    { };
};
} // namespace MultiplayerFeature
//------------------------------------------------------------------------------
