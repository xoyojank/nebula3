#pragma once
//------------------------------------------------------------------------------
/**
    @class ToolkitUtil::AppLauncher
    
    Launch an external Win32 application.
    
    (C) 2008 Radon Labs GmbH
*/
#include "util/string.h"
#include "io/uri.h"
#include "io/stream.h"

//------------------------------------------------------------------------------
namespace ToolkitUtil
{
class AppLauncher
{
public:
    
    /// constructor
    AppLauncher();

    /// set the executable path
    void SetExecutable(const IO::URI& uri);
    /// set working directory
    void SetWorkingDirectory(const IO::URI& uri);
    /// set command line arguments
    void SetArguments(const Util::String& args);
    /// do not open a console window for the new process
    void SetNoConsoleWindow(bool b);
    /// set optional stdout capture stream
    void SetStdoutCaptureStream(const Ptr<IO::Stream>& stream);
    /// get optional pointer to stdout capture stream
    const Ptr<IO::Stream>& GetStdoutCaptureStream() const;
    /// set optional stderr capture stream
    void SetStderrCaptureStream(const Ptr<IO::Stream>& stream);
    /// get optional pointer to stderr capture stream
    const Ptr<IO::Stream>& GetStderrCaptureStream() const;

    /// launch application and wait for its termination
    bool LaunchWait() const;
    /// launch application (returns immediately)
    bool Launch();
    /// Gets the state of application. Call this per frame.
    bool IsRunning();
    /// Read data from the captured stdout and writes it to the stream.
    void UpdateStdoutStream();

private:
    /// launch without stdout capturing
    bool LaunchWithoutOutputCapturing(DWORD creationFlags, const Util::String& cmdLine) const;
    /// launch with stdout capturing
    bool LaunchWithOutputCapturing(DWORD creationFlags, const Util::String& cmdLine) const;
    /// create pipe for optional stdout capturing
    bool CreateStdoutCapturePipe(PHANDLE stdoutRead, PHANDLE stdoutWrite) const;
    /// capture data from output
    void CaptureOutput(HANDLE stdoutRead, HANDLE stderrRead, HANDLE childProcess) const;

    bool isRunning;
    bool noConsoleWindow;
    IO::URI exePath;
    IO::URI workingDir;
    Util::String args;
    Ptr<IO::Stream> stdoutCaptureStream;
    Ptr<IO::Stream> stderrCaptureStream;
    
    PROCESS_INFORMATION lauchedProcessInfo;
    HANDLE stdoutAsyncRead;
    HANDLE stdoutAsyncWrite;
    HANDLE stderrAsyncRead;
    HANDLE stderrAsyncWrite;
    
    DWORD numAsyncBytesRead;
    DWORD numAsyncBytesAvailable;
    static const int asyncBufferSize = 4096;
    char asyncBuffer[asyncBufferSize];

};

//------------------------------------------------------------------------------
/**
*/
inline void
AppLauncher::SetExecutable(const IO::URI& uri)
{
    this->exePath = uri;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AppLauncher::SetWorkingDirectory(const IO::URI& uri)
{
    this->workingDir = uri;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AppLauncher::SetArguments(const Util::String& a)
{
    this->args = a;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AppLauncher::SetNoConsoleWindow(bool b)
{
    this->noConsoleWindow = b;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AppLauncher::SetStdoutCaptureStream(const Ptr<IO::Stream>& stream)
{
    this->stdoutCaptureStream = stream;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<IO::Stream>&
AppLauncher::GetStdoutCaptureStream() const
{
    return this->stdoutCaptureStream;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AppLauncher::SetStderrCaptureStream(const Ptr<IO::Stream>& stream)
{
    this->stderrCaptureStream = stream;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<IO::Stream>&
AppLauncher::GetStderrCaptureStream() const
{
    return this->stderrCaptureStream;
}


} // namespace ToolkitUtil
//------------------------------------------------------------------------------
    