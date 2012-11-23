#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "util/stringatom.h"
#include "math/float2.h"

//------------------------------------------------------------------------------
namespace SUI
{
//------------------------------------------------------------------------------
class SUIAction : public Messaging::Message
{
    __DeclareClass(SUIAction);
    __DeclareMsgId;
public:
    SUIAction() 
    { };
public:
    void SetLayoutId(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->layoutid = val;
    };
    const Util::StringAtom& GetLayoutId() const
    {
        return this->layoutid;
    };
private:
    Util::StringAtom layoutid;
public:
    void SetElementId(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->elementid = val;
    };
    const Util::StringAtom& GetElementId() const
    {
        return this->elementid;
    };
private:
    Util::StringAtom elementid;
};
//------------------------------------------------------------------------------
class SUIActionSetVisible : public SUIAction
{
    __DeclareClass(SUIActionSetVisible);
    __DeclareMsgId;
public:
    SUIActionSetVisible() 
    { };
public:
    void SetVisible(bool val)
    {
        n_assert(!this->handled);
        this->visible = val;
    };
    bool GetVisible() const
    {
        return this->visible;
    };
private:
    bool visible;
};
//------------------------------------------------------------------------------
class SUIActionSetSize : public SUIAction
{
    __DeclareClass(SUIActionSetSize);
    __DeclareMsgId;
public:
    SUIActionSetSize() 
    { };
public:
    void SetSize(const Math::float2& val)
    {
        n_assert(!this->handled);
        this->size = val;
    };
    const Math::float2& GetSize() const
    {
        return this->size;
    };
private:
    Math::float2 size;
};
//------------------------------------------------------------------------------
class SUIActionPlayLayoutAnim : public SUIAction
{
    __DeclareClass(SUIActionPlayLayoutAnim);
    __DeclareMsgId;
public:
    SUIActionPlayLayoutAnim() :
        resettime(true),
        reverse(false),
        speed(1.0)
    { };
public:
    void SetAnim(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->anim = val;
    };
    const Util::StringAtom& GetAnim() const
    {
        return this->anim;
    };
private:
    Util::StringAtom anim;
public:
    void SetResetTime(bool val)
    {
        n_assert(!this->handled);
        this->resettime = val;
    };
    bool GetResetTime() const
    {
        return this->resettime;
    };
private:
    bool resettime;
public:
    void SetReverse(bool val)
    {
        n_assert(!this->handled);
        this->reverse = val;
    };
    bool GetReverse() const
    {
        return this->reverse;
    };
private:
    bool reverse;
public:
    void SetSpeed(float val)
    {
        n_assert(!this->handled);
        this->speed = val;
    };
    float GetSpeed() const
    {
        return this->speed;
    };
private:
    float speed;
};
//------------------------------------------------------------------------------
class SUIActionPlayElementAnim : public SUIAction
{
    __DeclareClass(SUIActionPlayElementAnim);
    __DeclareMsgId;
public:
    SUIActionPlayElementAnim() :
        resettime(true),
        reverse(false),
        speed(1.0)
    { };
public:
    void SetAnim(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->anim = val;
    };
    const Util::StringAtom& GetAnim() const
    {
        return this->anim;
    };
private:
    Util::StringAtom anim;
public:
    void SetResetTime(bool val)
    {
        n_assert(!this->handled);
        this->resettime = val;
    };
    bool GetResetTime() const
    {
        return this->resettime;
    };
private:
    bool resettime;
public:
    void SetReverse(bool val)
    {
        n_assert(!this->handled);
        this->reverse = val;
    };
    bool GetReverse() const
    {
        return this->reverse;
    };
private:
    bool reverse;
public:
    void SetSpeed(float val)
    {
        n_assert(!this->handled);
        this->speed = val;
    };
    float GetSpeed() const
    {
        return this->speed;
    };
private:
    float speed;
};
//------------------------------------------------------------------------------
class SUIActionHasAnimFinished : public SUIAction
{
    __DeclareClass(SUIActionHasAnimFinished);
    __DeclareMsgId;
public:
    SUIActionHasAnimFinished() :
        result(false)
    { };
public:
    void SetAnim(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->anim = val;
    };
    const Util::StringAtom& GetAnim() const
    {
        return this->anim;
    };
private:
    Util::StringAtom anim;
public:
    void SetResult(bool val)
    {
        n_assert(!this->handled);
        this->result = val;
    };
    bool GetResult() const
    {
        n_assert(this->handled);
        return this->result;
    };
private:
    bool result;
};
//------------------------------------------------------------------------------
class SUIActionSetText : public SUIAction
{
    __DeclareClass(SUIActionSetText);
    __DeclareMsgId;
public:
    SUIActionSetText() 
    { };
public:
    void SetText(const Util::String& val)
    {
        n_assert(!this->handled);
        this->text = val;
    };
    const Util::String& GetText() const
    {
        return this->text;
    };
private:
    Util::String text;
public:
    void SetNewLineThreshold(int val)
    {
        n_assert(!this->handled);
        this->newlinethreshold = val;
    };
    int GetNewLineThreshold() const
    {
        return this->newlinethreshold;
    };
private:
    int newlinethreshold;
};
//------------------------------------------------------------------------------
class SUIActionSetTextColor : public SUIAction
{
    __DeclareClass(SUIActionSetTextColor);
    __DeclareMsgId;
public:
    SUIActionSetTextColor() 
    { };
public:
    void SetColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->color = val;
    };
    const Math::float4& GetColor() const
    {
        return this->color;
    };
private:
    Math::float4 color;
};
//------------------------------------------------------------------------------
class SUIActionSetFontSize : public SUIAction
{
    __DeclareClass(SUIActionSetFontSize);
    __DeclareMsgId;
public:
    SUIActionSetFontSize() 
    { };
public:
    void SetSizeX(float val)
    {
        n_assert(!this->handled);
        this->sizex = val;
    };
    float GetSizeX() const
    {
        return this->sizex;
    };
private:
    float sizex;
public:
    void SetSizeY(float val)
    {
        n_assert(!this->handled);
        this->sizey = val;
    };
    float GetSizeY() const
    {
        return this->sizey;
    };
private:
    float sizey;
};
//------------------------------------------------------------------------------
class SUIActionSetPosition : public SUIAction
{
    __DeclareClass(SUIActionSetPosition);
    __DeclareMsgId;
public:
    SUIActionSetPosition() 
    { };
public:
    void SetSizeX(float val)
    {
        n_assert(!this->handled);
        this->sizex = val;
    };
    float GetSizeX() const
    {
        return this->sizex;
    };
private:
    float sizex;
public:
    void SetSizeY(float val)
    {
        n_assert(!this->handled);
        this->sizey = val;
    };
    float GetSizeY() const
    {
        return this->sizey;
    };
private:
    float sizey;
};
//------------------------------------------------------------------------------
class SUIActionGetPosition : public SUIAction
{
    __DeclareClass(SUIActionGetPosition);
    __DeclareMsgId;
public:
    SUIActionGetPosition() 
    { };
public:
    void SetPosition(const Math::float2& val)
    {
        n_assert(!this->handled);
        this->position = val;
    };
    const Math::float2& GetPosition() const
    {
        n_assert(this->handled);
        return this->position;
    };
private:
    Math::float2 position;
};
//------------------------------------------------------------------------------
class SUIActionSetPaused : public SUIAction
{
    __DeclareClass(SUIActionSetPaused);
    __DeclareMsgId;
public:
    SUIActionSetPaused() 
    { };
public:
    void SetPaused(bool val)
    {
        n_assert(!this->handled);
        this->paused = val;
    };
    bool GetPaused() const
    {
        return this->paused;
    };
private:
    bool paused;
};
} // namespace SUI
//------------------------------------------------------------------------------
