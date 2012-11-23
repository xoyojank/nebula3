#pragma once
//------------------------------------------------------------------------------
/**
    @class ToolkitUtil::ShaderCompiler
    
    Wraps the shader compilation process for all supported target platforms.
        
    (C) 2008 Radon Labs GmbH
*/
#include "toolkitutil/platform.h"
#include "io/uri.h"
#include "util/string.h"

//------------------------------------------------------------------------------
namespace ToolkitUtil
{
class ShaderCompiler
{
public:
    /// constructor
    ShaderCompiler();
    /// destructor
    ~ShaderCompiler();

    /// set target platform
    void SetPlatform(Platform::Code platform);
    /// set source directory
    void SetShaderSrcDir(const Util::String& srcDir);
    /// set destination directory
    void SetShaderDstDir(const Util::String& dstDir);
    /// set frame shader source dir
    void SetFrameShaderSrcDir(const Util::String& srcDir);
    /// set frame shader destination dir
    void SetFrameShaderDstDir(const Util::String& dstDir);
    /// set path to compile tool
    void SetToolPath(const Util::String& toolPath);
    /// set force conversion flag (otherwise check timestamps)
    void SetForceFlag(bool b);
    /// set debugging flag
    void SetDebugFlag(bool b);
    /// set additional command line params
    void SetAdditionalParams(const Util::String& params);
    /// set quiet flag
    void SetQuietFlag(bool b);

    /// compile all shaders 
    bool CompileShaders();
    /// compile frame shader files
    bool CompileFrameShaders();

private:
    /// check whether a file needs a recompile (force flag and timestamps)
    bool CheckRecompile(const Util::String& srcPath, const Util::String& dstPath);
    /// compile Wii shaders
    bool CompileShadersWii();
    /// compile shaders in Nebula2 mode
    bool CompileShadersNebula2();
    /// compile shaders for Win32 and Xbox360 platform
    bool CompileShadersWin360();
    /// compile shaders for PS3 platform
    bool CompileShadersPS3();
    /// compile a single shader for Win32, Xbox360 or Nebula2 (N2 mode called from CompileShaderNebula2())
    bool CompileShaderWin360(const Util::String& srcFile, const Util::String& dstFile, const Util::String& extraParams);
    /// compile a single shader in N2 mode (calls CompileShaderWin360)
    bool CompileShaderNebula2(const Util::String& srcFile, const Util::String& dstFile);
    /// compile a single shader for PS3
    bool CompileCgShaderPS3(const Util::String& srcFile, const Util::String& dstFile, const Util::String& defines, bool isFragmentShader);
    /// write shader dictionary
    bool WriteShaderDictionary();

    Platform::Code platform;
    Util::String srcShaderDir;
    Util::String dstShaderDir;
    Util::String srcFrameShaderDir;
    Util::String dstFrameShaderDir;
    Util::String toolPath;
    bool force;
    bool quiet;
    bool debug;
    Util::String additionalParams;
    Util::Array<Util::String> shaderNames;
};

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderCompiler::SetPlatform(Platform::Code p)
{
    this->platform = p;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderCompiler::SetShaderSrcDir(const Util::String& d)
{
    this->srcShaderDir = d;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderCompiler::SetShaderDstDir(const Util::String& d)
{
    this->dstShaderDir = d;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderCompiler::SetFrameShaderSrcDir(const Util::String& d)
{
    this->srcFrameShaderDir = d;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderCompiler::SetFrameShaderDstDir(const Util::String& d)
{
    this->dstFrameShaderDir = d;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderCompiler::SetToolPath(const Util::String& p)
{
    this->toolPath = p;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderCompiler::SetForceFlag(bool b)
{
    this->force = b;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderCompiler::SetDebugFlag(bool b)
{
    this->debug = b;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderCompiler::SetAdditionalParams(const Util::String& p)
{
    this->additionalParams = p;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderCompiler::SetQuietFlag(bool b)
{
    this->quiet = b;
}

} // namespace ToolkitUtil
//------------------------------------------------------------------------------
 