#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "util/stringatom.h"
#include "math/bbox.h"
#include "math/matrix44.h"

//------------------------------------------------------------------------------
namespace Forest
{
//------------------------------------------------------------------------------
class LoadLevel : public Messaging::Message
{
    __DeclareClass(LoadLevel);
    __DeclareMsgId;
public:
    LoadLevel() 
    { };
public:
    void SetBoundingBox(const Math::bbox& val)
    {
        n_assert(!this->handled);
        this->boundingbox = val;
    };
    const Math::bbox& GetBoundingBox() const
    {
        return this->boundingbox;
    };
private:
    Math::bbox boundingbox;
};
//------------------------------------------------------------------------------
class ExitLevel : public Messaging::Message
{
    __DeclareClass(ExitLevel);
    __DeclareMsgId;
public:
    ExitLevel() 
    { };
};
//------------------------------------------------------------------------------
class CreateTreeInstance : public Messaging::Message
{
    __DeclareClass(CreateTreeInstance);
    __DeclareMsgId;
public:
    CreateTreeInstance() 
    { };
public:
    void SetResourceId(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->resourceid = val;
    };
    const Util::StringAtom& GetResourceId() const
    {
        return this->resourceid;
    };
private:
    Util::StringAtom resourceid;
public:
    void SetTreeId(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->treeid = val;
    };
    const Util::StringAtom& GetTreeId() const
    {
        return this->treeid;
    };
private:
    Util::StringAtom treeid;
public:
    void SetTransform(const Math::matrix44& val)
    {
        n_assert(!this->handled);
        this->transform = val;
    };
    const Math::matrix44& GetTransform() const
    {
        return this->transform;
    };
private:
    Math::matrix44 transform;
public:
    void SetCollideFile(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->collidefile = val;
    };
    const Util::StringAtom& GetCollideFile() const
    {
        return this->collidefile;
    };
private:
    Util::StringAtom collidefile;
};
//------------------------------------------------------------------------------
class SetupCollideMesh : public Messaging::Message
{
    __DeclareClass(SetupCollideMesh);
    __DeclareMsgId;
public:
    SetupCollideMesh() 
    { };
};
} // namespace Forest
//------------------------------------------------------------------------------
