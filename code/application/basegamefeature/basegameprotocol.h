#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "game/entity.h"
#include "math/vector.h"
#include "basegameattr/basegameattributes.h"
#include "math/quaternion.h"

//------------------------------------------------------------------------------
namespace BaseGameFeature
{
//------------------------------------------------------------------------------
class MoveDirection : public Messaging::Message
{
    __DeclareClass(MoveDirection);
    __DeclareMsgId;
public:
    MoveDirection() :
        direction(1.0f, 0.0f, 0.0f),
        maxmovement(1.0f),
        camerarelative(false)
    { };
public:
    void SetDirection(const Math::vector& val)
    {
        n_assert(!this->handled);
        this->direction = val;
    };
    const Math::vector& GetDirection() const
    {
        return this->direction;
    };
private:
    Math::vector direction;
public:
    void SetMaxMovement(float val)
    {
        n_assert(!this->handled);
        this->maxmovement = val;
    };
    float GetMaxMovement() const
    {
        return this->maxmovement;
    };
private:
    float maxmovement;
public:
    void SetCameraRelative(bool val)
    {
        n_assert(!this->handled);
        this->camerarelative = val;
    };
    bool GetCameraRelative() const
    {
        return this->camerarelative;
    };
private:
    bool camerarelative;
};
//------------------------------------------------------------------------------
class MoveFollow : public Messaging::Message
{
    __DeclareClass(MoveFollow);
    __DeclareMsgId;
public:
    MoveFollow() :
        targetentityid(0),
        distance(1.0f)
    { };
public:
    void SetTargetEntityId(const Game::Entity::EntityId& val)
    {
        n_assert(!this->handled);
        this->targetentityid = val;
    };
    const Game::Entity::EntityId& GetTargetEntityId() const
    {
        return this->targetentityid;
    };
private:
    Game::Entity::EntityId targetentityid;
public:
    void SetDistance(float val)
    {
        n_assert(!this->handled);
        this->distance = val;
    };
    float GetDistance() const
    {
        return this->distance;
    };
private:
    float distance;
};
//------------------------------------------------------------------------------
class MoveGoto : public Messaging::Message
{
    __DeclareClass(MoveGoto);
    __DeclareMsgId;
public:
    MoveGoto() :
        distance(0.5f)
    { };
public:
    void SetPosition(const Math::vector& val)
    {
        n_assert(!this->handled);
        this->position = val;
    };
    const Math::vector& GetPosition() const
    {
        return this->position;
    };
private:
    Math::vector position;
public:
    void SetDistance(float val)
    {
        n_assert(!this->handled);
        this->distance = val;
    };
    float GetDistance() const
    {
        return this->distance;
    };
private:
    float distance;
};
//------------------------------------------------------------------------------
class MoveRotate : public Messaging::Message
{
    __DeclareClass(MoveRotate);
    __DeclareMsgId;
public:
    MoveRotate() :
        angle(0.0f)
    { };
public:
    void SetAngle(float val)
    {
        n_assert(!this->handled);
        this->angle = val;
    };
    float GetAngle() const
    {
        return this->angle;
    };
private:
    float angle;
};
//------------------------------------------------------------------------------
class MoveSetVelocity : public Messaging::Message
{
    __DeclareClass(MoveSetVelocity);
    __DeclareMsgId;
public:
    MoveSetVelocity() :
        relvelocity(1.0f)
    { };
public:
    void SetRelVelocity(float val)
    {
        n_assert(!this->handled);
        this->relvelocity = val;
    };
    float GetRelVelocity() const
    {
        return this->relvelocity;
    };
private:
    float relvelocity;
};
//------------------------------------------------------------------------------
class MoveStop : public Messaging::Message
{
    __DeclareClass(MoveStop);
    __DeclareMsgId;
public:
    MoveStop() 
    { };
};
//------------------------------------------------------------------------------
class MoveTurn : public Messaging::Message
{
    __DeclareClass(MoveTurn);
    __DeclareMsgId;
public:
    MoveTurn() :
        direction(1.0f, 0.0f, 0.0f),
        camerarelative(false)
    { };
public:
    void SetDirection(const Math::vector& val)
    {
        n_assert(!this->handled);
        this->direction = val;
    };
    const Math::vector& GetDirection() const
    {
        return this->direction;
    };
private:
    Math::vector direction;
public:
    void SetEntities(const Ptr<Game::Entity>& val)
    {
        n_assert(!this->handled);
        this->entities = val;
    };
    const Ptr<Game::Entity>& GetEntities() const
    {
        return this->entities;
    };
private:
    Ptr<Game::Entity> entities;
public:
    void SetCameraRelative(bool val)
    {
        n_assert(!this->handled);
        this->camerarelative = val;
    };
    bool GetCameraRelative() const
    {
        return this->camerarelative;
    };
private:
    bool camerarelative;
};
//------------------------------------------------------------------------------
class UpdateTransform : public Messaging::Message
{
    __DeclareClass(UpdateTransform);
    __DeclareMsgId;
public:
    UpdateTransform() :
        smoothing(true)
    { };
public:
    void SetMatrix(const Math::matrix44& val)
    {
        n_assert(!this->handled);
        this->matrix = val;
    };
    const Math::matrix44& GetMatrix() const
    {
        return this->matrix;
    };
private:
    Math::matrix44 matrix;
public:
    void SetSmoothing(bool val)
    {
        n_assert(!this->handled);
        this->smoothing = val;
    };
    bool GetSmoothing() const
    {
        return this->smoothing;
    };
private:
    bool smoothing;
};
//------------------------------------------------------------------------------
class SetTransform : public UpdateTransform
{
    __DeclareClass(SetTransform);
    __DeclareMsgId;
public:
    SetTransform() 
    { };
};
//------------------------------------------------------------------------------
class EnableAmbienceBubble : public Messaging::Message
{
    __DeclareClass(EnableAmbienceBubble);
    __DeclareMsgId;
public:
    EnableAmbienceBubble() 
    { };
public:
    void SetEnabled(bool val)
    {
        n_assert(!this->handled);
        this->enabled = val;
    };
    bool GetEnabled() const
    {
        return this->enabled;
    };
private:
    bool enabled;
};
//------------------------------------------------------------------------------
class AttributesUpdated : public Messaging::Message
{
    __DeclareClass(AttributesUpdated);
    __DeclareMsgId;
public:
    AttributesUpdated() 
    { };
};
} // namespace BaseGameFeature
//------------------------------------------------------------------------------
