#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "util/stringatom.h"

//------------------------------------------------------------------------------
namespace Video
{
//------------------------------------------------------------------------------
class PlayVideo : public Messaging::Message
{
    __DeclareClass(PlayVideo);
    __DeclareMsgId;
public:
    PlayVideo() 
    { };
public:
    void SetResourceName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->resourcename = val;
    };
    const Util::StringAtom& GetResourceName() const
    {
        return this->resourcename;
    };
private:
    Util::StringAtom resourcename;
public:
    void SetFromFile(bool val)
    {
        n_assert(!this->handled);
        this->fromfile = val;
    };
    bool GetFromFile() const
    {
        return this->fromfile;
    };
private:
    bool fromfile;
public:
    void SetAutoDelete(bool val)
    {
        n_assert(!this->handled);
        this->autodelete = val;
    };
    bool GetAutoDelete() const
    {
        return this->autodelete;
    };
private:
    bool autodelete;
public:
    void SetLoop(bool val)
    {
        n_assert(!this->handled);
        this->loop = val;
    };
    bool GetLoop() const
    {
        return this->loop;
    };
private:
    bool loop;
public:
    void SetUpperLeftCorner(const Math::float2& val)
    {
        n_assert(!this->handled);
        this->upperleftcorner = val;
    };
    const Math::float2& GetUpperLeftCorner() const
    {
        return this->upperleftcorner;
    };
private:
    Math::float2 upperleftcorner;
public:
    void SetUpperRightCorner(const Math::float2& val)
    {
        n_assert(!this->handled);
        this->upperrightcorner = val;
    };
    const Math::float2& GetUpperRightCorner() const
    {
        return this->upperrightcorner;
    };
private:
    Math::float2 upperrightcorner;
public:
    void SetLowerLeftCorner(const Math::float2& val)
    {
        n_assert(!this->handled);
        this->lowerleftcorner = val;
    };
    const Math::float2& GetLowerLeftCorner() const
    {
        return this->lowerleftcorner;
    };
private:
    Math::float2 lowerleftcorner;
public:
    void SetLowerRightCorner(const Math::float2& val)
    {
        n_assert(!this->handled);
        this->lowerrightcorner = val;
    };
    const Math::float2& GetLowerRightCorner() const
    {
        return this->lowerrightcorner;
    };
private:
    Math::float2 lowerrightcorner;
};
//------------------------------------------------------------------------------
class StopVideo : public Messaging::Message
{
    __DeclareClass(StopVideo);
    __DeclareMsgId;
public:
    StopVideo() 
    { };
public:
    void SetResourceName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->resourcename = val;
    };
    const Util::StringAtom& GetResourceName() const
    {
        return this->resourcename;
    };
private:
    Util::StringAtom resourcename;
public:
    void SetDeleteVideo(bool val)
    {
        n_assert(!this->handled);
        this->deletevideo = val;
    };
    bool GetDeleteVideo() const
    {
        return this->deletevideo;
    };
private:
    bool deletevideo;
};
//------------------------------------------------------------------------------
class PauseVideo : public Messaging::Message
{
    __DeclareClass(PauseVideo);
    __DeclareMsgId;
public:
    PauseVideo() 
    { };
public:
    void SetResourceName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->resourcename = val;
    };
    const Util::StringAtom& GetResourceName() const
    {
        return this->resourcename;
    };
private:
    Util::StringAtom resourcename;
};
//------------------------------------------------------------------------------
class ResumeVideo : public Messaging::Message
{
    __DeclareClass(ResumeVideo);
    __DeclareMsgId;
public:
    ResumeVideo() 
    { };
public:
    void SetResourceName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->resourcename = val;
    };
    const Util::StringAtom& GetResourceName() const
    {
        return this->resourcename;
    };
private:
    Util::StringAtom resourcename;
};
//------------------------------------------------------------------------------
class DeleteVideo : public Messaging::Message
{
    __DeclareClass(DeleteVideo);
    __DeclareMsgId;
public:
    DeleteVideo() 
    { };
public:
    void SetResourceName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->resourcename = val;
    };
    const Util::StringAtom& GetResourceName() const
    {
        return this->resourcename;
    };
private:
    Util::StringAtom resourcename;
};
//------------------------------------------------------------------------------
class IsVideoPlaying : public Messaging::Message
{
    __DeclareClass(IsVideoPlaying);
    __DeclareMsgId;
public:
    IsVideoPlaying() 
    { };
public:
    void SetResourceName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->resourcename = val;
    };
    const Util::StringAtom& GetResourceName() const
    {
        return this->resourcename;
    };
private:
    Util::StringAtom resourcename;
public:
    void SetPlaying(bool val)
    {
        n_assert(!this->handled);
        this->playing = val;
    };
    bool GetPlaying() const
    {
        n_assert(this->handled);
        return this->playing;
    };
private:
    bool playing;
};
} // namespace Video
//------------------------------------------------------------------------------
