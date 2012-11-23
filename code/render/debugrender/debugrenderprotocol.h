#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "util/array.h"
#include "threading/threadid.h"
#include "coregraphics/textelement.h"
#include "debugrender/debugshaperenderer.h"
#include "coregraphics/shadervariable.h"

//------------------------------------------------------------------------------
namespace Debug
{
//------------------------------------------------------------------------------
class RenderDebugText : public Messaging::Message
{
    __DeclareClass(RenderDebugText);
    __DeclareMsgId;
public:
    RenderDebugText() 
    { };
public:
    void SetThreadId(const Threading::ThreadId& val)
    {
        n_assert(!this->handled);
        this->threadid = val;
    };
    const Threading::ThreadId& GetThreadId() const
    {
        return this->threadid;
    };
private:
    Threading::ThreadId threadid;
public:
    void SetTextElements(const Util::Array<CoreGraphics::TextElement>& val)
    {
        n_assert(!this->handled);
        this->textelements = val;
    };
    const Util::Array<CoreGraphics::TextElement>& GetTextElements() const
    {
        return this->textelements;
    };
private:
    Util::Array<CoreGraphics::TextElement> textelements;
};
//------------------------------------------------------------------------------
class RenderDebugShapes : public Messaging::Message
{
    __DeclareClass(RenderDebugShapes);
    __DeclareMsgId;
public:
    RenderDebugShapes() 
    { };
public:
    void SetThreadId(const Threading::ThreadId& val)
    {
        n_assert(!this->handled);
        this->threadid = val;
    };
    const Threading::ThreadId& GetThreadId() const
    {
        return this->threadid;
    };
private:
    Threading::ThreadId threadid;
public:
    void SetShapes(const Util::Array<CoreGraphics::RenderShape>& val)
    {
        n_assert(!this->handled);
        this->shapes = val;
    };
    const Util::Array<CoreGraphics::RenderShape>& GetShapes() const
    {
        return this->shapes;
    };
private:
    Util::Array<CoreGraphics::RenderShape> shapes;
};
//------------------------------------------------------------------------------
class RenderDebugView : public Messaging::Message
{
    __DeclareClass(RenderDebugView);
    __DeclareMsgId;
public:
    RenderDebugView() 
    { };
public:
    void SetThreadId(const Threading::ThreadId& val)
    {
        n_assert(!this->handled);
        this->threadid = val;
    };
    const Threading::ThreadId& GetThreadId() const
    {
        return this->threadid;
    };
private:
    Threading::ThreadId threadid;
public:
    void SetEnableDebugRendering(bool val)
    {
        n_assert(!this->handled);
        this->enabledebugrendering = val;
    };
    bool GetEnableDebugRendering() const
    {
        return this->enabledebugrendering;
    };
private:
    bool enabledebugrendering;
};
//------------------------------------------------------------------------------
class IncrDebugShaderLayer : public Messaging::Message
{
    __DeclareClass(IncrDebugShaderLayer);
    __DeclareMsgId;
public:
    IncrDebugShaderLayer() 
    { };
public:
    void SetThreadId(const Threading::ThreadId& val)
    {
        n_assert(!this->handled);
        this->threadid = val;
    };
    const Threading::ThreadId& GetThreadId() const
    {
        return this->threadid;
    };
private:
    Threading::ThreadId threadid;
};
//------------------------------------------------------------------------------
class SetFloatShaderVariable : public Messaging::Message
{
    __DeclareClass(SetFloatShaderVariable);
    __DeclareMsgId;
public:
    SetFloatShaderVariable() 
    { };
public:
    void SetSemantic(const CoreGraphics::ShaderVariable::Semantic& val)
    {
        n_assert(!this->handled);
        this->semantic = val;
    };
    const CoreGraphics::ShaderVariable::Semantic& GetSemantic() const
    {
        return this->semantic;
    };
private:
    CoreGraphics::ShaderVariable::Semantic semantic;
public:
    void SetValue(float val)
    {
        n_assert(!this->handled);
        this->value = val;
    };
    float GetValue() const
    {
        return this->value;
    };
private:
    float value;
};
//------------------------------------------------------------------------------
class ControlPerfHUD : public Messaging::Message
{
    __DeclareClass(ControlPerfHUD);
    __DeclareMsgId;
public:
    ControlPerfHUD() :
        toggle(false),
        enable(false)
    { };
public:
    void SetToggle(bool val)
    {
        n_assert(!this->handled);
        this->toggle = val;
    };
    bool GetToggle() const
    {
        return this->toggle;
    };
private:
    bool toggle;
public:
    void SetEnable(bool val)
    {
        n_assert(!this->handled);
        this->enable = val;
    };
    bool GetEnable() const
    {
        return this->enable;
    };
private:
    bool enable;
};
} // namespace Debug
//------------------------------------------------------------------------------
