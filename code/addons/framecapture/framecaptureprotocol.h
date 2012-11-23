#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "timing/time.h"
#include "coregraphics/imagefileformat.h"

//------------------------------------------------------------------------------
namespace FrameCapture
{
//------------------------------------------------------------------------------
class SetupFrameCapture : public Messaging::Message
{
    __DeclareClass(SetupFrameCapture);
    __DeclareMsgId;
public:
    SetupFrameCapture() 
    { };
public:
    void SetBaseDirectory(const Util::String& val)
    {
        n_assert(!this->handled);
        this->basedirectory = val;
    };
    const Util::String& GetBaseDirectory() const
    {
        return this->basedirectory;
    };
private:
    Util::String basedirectory;
public:
    void SetFrameTime(const Timing::Time& val)
    {
        n_assert(!this->handled);
        this->frametime = val;
    };
    const Timing::Time& GetFrameTime() const
    {
        return this->frametime;
    };
private:
    Timing::Time frametime;
public:
    void SetCaptureFileFormat(const CoreGraphics::ImageFileFormat::Code& val)
    {
        n_assert(!this->handled);
        this->capturefileformat = val;
    };
    const CoreGraphics::ImageFileFormat::Code& GetCaptureFileFormat() const
    {
        return this->capturefileformat;
    };
private:
    CoreGraphics::ImageFileFormat::Code capturefileformat;
};
//------------------------------------------------------------------------------
class GetFrameCaptureSetup : public Messaging::Message
{
    __DeclareClass(GetFrameCaptureSetup);
    __DeclareMsgId;
public:
    GetFrameCaptureSetup() 
    { };
public:
    void SetBaseDirectory(const Util::String& val)
    {
        n_assert(!this->handled);
        this->basedirectory = val;
    };
    const Util::String& GetBaseDirectory() const
    {
        n_assert(this->handled);
        return this->basedirectory;
    };
private:
    Util::String basedirectory;
public:
    void SetFrameTime(const Timing::Time& val)
    {
        n_assert(!this->handled);
        this->frametime = val;
    };
    const Timing::Time& GetFrameTime() const
    {
        n_assert(this->handled);
        return this->frametime;
    };
private:
    Timing::Time frametime;
public:
    void SetCaptureFileFormat(const CoreGraphics::ImageFileFormat::Code& val)
    {
        n_assert(!this->handled);
        this->capturefileformat = val;
    };
    const CoreGraphics::ImageFileFormat::Code& GetCaptureFileFormat() const
    {
        n_assert(this->handled);
        return this->capturefileformat;
    };
private:
    CoreGraphics::ImageFileFormat::Code capturefileformat;
};
//------------------------------------------------------------------------------
class StartFrameCapture : public Messaging::Message
{
    __DeclareClass(StartFrameCapture);
    __DeclareMsgId;
public:
    StartFrameCapture() 
    { };
};
//------------------------------------------------------------------------------
class StopFrameCapture : public Messaging::Message
{
    __DeclareClass(StopFrameCapture);
    __DeclareMsgId;
public:
    StopFrameCapture() 
    { };
};
//------------------------------------------------------------------------------
class SaveScreenShot : public Messaging::Message
{
    __DeclareClass(SaveScreenShot);
    __DeclareMsgId;
public:
    SaveScreenShot() 
    { };
};
} // namespace FrameCapture
//------------------------------------------------------------------------------
