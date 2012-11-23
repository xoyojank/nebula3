//------------------------------------------------------------------------------
//  shadercompiler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "toolkitutil/shadercompiler.h"
#include "toolkitutil/applauncher.h"
#include "io/ioserver.h"
#include "io/xmlreader.h"

namespace ToolkitUtil
{
using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
ShaderCompiler::ShaderCompiler() :
    platform(Platform::Win32),
    force(false),
    debug(false),
    quiet(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ShaderCompiler::~ShaderCompiler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Check whether a files needs to be recompiled.
*/
bool
ShaderCompiler::CheckRecompile(const String& srcPath, const String& dstPath)
{
    if (!this->force)
    {
        // check file stamps
        IoServer* ioServer = IoServer::Instance();
        if (ioServer->FileExists(dstPath))
        {
            FileTime srcTime = ioServer->GetFileWriteTime(srcPath);
            FileTime dstTime = ioServer->GetFileWriteTime(dstPath);
            if (dstTime > srcTime)
            {
                return false;
            }
        }
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
ShaderCompiler::CompileShaders()
{
    n_assert(this->srcShaderDir.IsValid());
    n_assert(this->dstShaderDir.IsValid());
    IoServer* ioServer = IoServer::Instance();
    this->shaderNames.Clear();

    // check if compile tool path is set
    if ((Platform::Wii != this->platform) && (!this->toolPath.IsValid()))
    {
        n_printf("WARNING: no compile tool set!\n");
        return false;
    }

    // check if source dir exists
    if (!ioServer->DirectoryExists(this->srcShaderDir))
    {
        n_printf("WARNING: shader source directory '%s' not found!\n", this->srcShaderDir.AsCharPtr());
        return false;
    }

    // make sure the target directory exists
    ioServer->CreateDirectory(this->dstShaderDir);

    // by platform
    bool retval;
    if (Platform::Nebula2 == this->platform)
    {
        retval = this->CompileShadersNebula2();
    }
    else if (Platform::Wii == this->platform)
    {
        retval = this->CompileShadersWii();
    }
    else if ((Platform::Xbox360 == this->platform) || (Platform::Win32 == this->platform))
    {
        retval = this->CompileShadersWin360();
    }
    else if (Platform::PS3 == this->platform)
    {
        retval = this->CompileShadersPS3();
    }
    else
    {
        n_printf("Unsupported platform '%s'\n", Platform::ToString(this->platform).AsCharPtr());
        return false;
    }

    // write shader dictionary file
    if (retval)
    {
        retval = this->WriteShaderDictionary();
    }
    return retval;
}

//------------------------------------------------------------------------------
/**
    On the Wii, N3 shaders are simply N3 XML files with render state
    definitions.
*/
bool
ShaderCompiler::CompileShadersWii()
{
    IoServer* ioServer = IoServer::Instance();

    // simply copy the XML files over to the target directory
    bool retval = true;
    Array<String> srcFiles = ioServer->ListFiles(this->srcShaderDir, "*.xml");
    IndexT i;
    for (i = 0; i < srcFiles.Size(); i++)
    {
        this->shaderNames.Append(srcFiles[i]);
        this->shaderNames.Back().StripFileExtension();

        String srcPath;
        srcPath.Format("%s/%s", this->srcShaderDir.AsCharPtr(), srcFiles[i].AsCharPtr());
        String dstPath;
        dstPath.Format("%s/%s", this->dstShaderDir.AsCharPtr(), srcFiles[i].AsCharPtr());
        dstPath.StripFileExtension();
        if (!ioServer->CopyFile(srcPath, dstPath))
        {
            n_printf("Failed to copy file '%s' to '%s'!\n", srcPath.AsCharPtr(), dstPath.AsCharPtr());
            retval = false;
        }
    }
    return retval;
}

//------------------------------------------------------------------------------
/**
    Compile all shaders for N2 legacy mode.
*/
bool 
ShaderCompiler::CompileShadersNebula2()
{
    bool retval = true;
    Array<String> srcFiles = IoServer::Instance()->ListFiles(this->srcShaderDir, "*.fx");
    IndexT i;
    for (i = 0; i < srcFiles.Size(); i++)
    {
        String dstFile = srcFiles[i];
        dstFile.StripFileExtension();
        this->shaderNames.Append(dstFile);

        if (!this->CompileShaderNebula2(srcFiles[i], dstFile))
        {
            n_printf("Failed to compile shader '%s'!\n", srcFiles[i].AsCharPtr());
            retval = false;
        }
    }
    return retval;
}

//------------------------------------------------------------------------------
/**
    Compile all shaders for the N3/Win32+Xbox360 platform.
*/
bool 
ShaderCompiler::CompileShadersWin360()
{   
    bool retval = true;
    Array<String> srcFiles = IoServer::Instance()->ListFiles(this->srcShaderDir, "*.fx");
    IndexT i;
    for (i = 0; i < srcFiles.Size(); i++)
    {
        String dstFile = srcFiles[i];
        dstFile.StripFileExtension();
        this->shaderNames.Append(dstFile);

        if (!this->CompileShaderWin360(srcFiles[i], dstFile, ""))
        {
            n_printf("Failed to compile shader '%s'!\n", srcFiles[i].AsCharPtr());
            retval = false;
        }
    }
    return retval;
}

//------------------------------------------------------------------------------
/**
    Compile all shaders for the N3/PS3 platform.
*/
bool
ShaderCompiler::CompileShadersPS3()
{
    IoServer* ioServer = IoServer::Instance();

    // make sure the cg subdirectory exists in the destination directory
    ioServer->CreateDirectory(this->dstShaderDir + "/cg");

    // copy the XML files
    bool retval = true;
    Array<String> srcFiles = IoServer::Instance()->ListFiles(this->srcShaderDir, "*.xml");
    IndexT i;
    for (i = 0; i < srcFiles.Size(); i++)
    {
        // shared.xml is a special file, not a shader file
        if (srcFiles[i] != "shared.xml")
        {
            this->shaderNames.Append(srcFiles[i]);
            this->shaderNames.Back().StripFileExtension();
        }

        String srcPath;
        srcPath.Format("%s/%s", this->srcShaderDir.AsCharPtr(), srcFiles[i].AsCharPtr());
        String dstPath;
        dstPath.Format("%s/%s", this->dstShaderDir.AsCharPtr(), srcFiles[i].AsCharPtr());
        dstPath.StripFileExtension();
        if (!ioServer->CopyFile(srcPath, dstPath))
        {
            n_printf("Failed to copy file '%s' to '%s'!\n", srcPath.AsCharPtr(), dstPath.AsCharPtr());
            retval = false;
        }

        // parse the file and compile Cg programs
        Ptr<Stream> stream = ioServer->CreateStream(srcPath);
        Ptr<XmlReader> xmlReader = XmlReader::Create();
        xmlReader->SetStream(stream);
        if (xmlReader->Open())
        {
            if (xmlReader->HasNode("/PS3Shader"))
            {
                xmlReader->SetToNode("/PS3Shader");
                if (xmlReader->SetToFirstChild("Variation")) do
                {
                    if (xmlReader->HasNode("VertexShader"))
                    {
                        xmlReader->SetToNode("VertexShader");
                        {
                            String srcFile = xmlReader->GetString("src");
                            String dstFile = xmlReader->GetString("dst");
                            String def = xmlReader->GetString("def");
                            if (!this->CompileCgShaderPS3(srcFile, dstFile, def, false))
                            {
                                return false;
                            }
                        }
                        xmlReader->SetToParent();
                    }
                    if (xmlReader->HasNode("PixelShader"))
                    {
                        xmlReader->SetToNode("PixelShader");
                        {
                            String srcFile = xmlReader->GetString("src");
                            String dstFile = xmlReader->GetString("dst");
                            String def = xmlReader->GetString("def");
                            if (!this->CompileCgShaderPS3(srcFile, dstFile, def, true))
                            {
                                return false;
                            }
                        }
                        xmlReader->SetToParent();
                    }
                }
                while (xmlReader->SetToNextChild("Variation"));
            }
            xmlReader->Close();
        }
    }
    return retval;
}

//------------------------------------------------------------------------------
/**
    Compile a single shader for Nebula2, Nebula3 Win32, or Nebula3 Xbox360.
*/
bool
ShaderCompiler::CompileShaderWin360(const String& srcFile, const String& dstFile, const String& extraParams)
{
    n_assert((Platform::Xbox360 == this->platform) || (Platform::Win32 == this->platform) || (Platform::Nebula2 == this->platform));
    n_assert(srcFile.IsValid());
    IoServer* ioServer = IoServer::Instance();

    // build an absolute filename to the src file
    String srcPath;
    srcPath.Format("%s/%s", this->srcShaderDir.AsCharPtr(), srcFile.AsCharPtr());
    String resolvedSrcPath = AssignRegistry::Instance()->ResolveAssignsInString(srcPath);

    // construct target file name 
    String dstPath;
    dstPath.Format("%s/%s", this->dstShaderDir.AsCharPtr(), dstFile.AsCharPtr());
    dstPath = AssignRegistry::Instance()->ResolveAssignsInString(dstPath);

    // check if file needs recompilation
    bool compile = this->CheckRecompile(srcPath, dstPath);
    if (!compile)
    {
        return true;
    }

    // Xbox360/Win32: build shader args
    String args = "/nologo ";
    if (Platform::Xbox360 == this->platform)
    {
        // Xbox360 platform
        args.Append("/XOautoz /T fxl_3_0 /D__XBOX360__=1 ");
        if (this->debug)
        {
            // build a filename for the debug info pdb file
            String pdbPath = this->dstShaderDir;
            String resolvedPdbPath = AssignRegistry::Instance()->ResolveAssignsInString(pdbPath);

            resolvedPdbPath.Append("/");
            resolvedPdbPath.Append(srcFile);
            resolvedPdbPath.StripFileExtension();
            resolvedPdbPath.Append(".updb");
            
            args.Append("/XZi ");
            args.Append("/XFd ");
            args.Append(resolvedPdbPath);
            args.Append(" ");
        }
    }
    else
    {
        // Win32 platform
        args.Append("/T fx_2_0 /D__WIN32__=1 ");
        if (this->debug)
        {
            args.Append("/Od /Zi ");
            args.Append("/Fc  ");

            String pdbPath = this->dstShaderDir;
            String assemblerCodeFile = AssignRegistry::Instance()->ResolveAssignsInString(pdbPath);
            assemblerCodeFile.Append("/");
            assemblerCodeFile.Append(srcFile);
            assemblerCodeFile.StripFileExtension();
            assemblerCodeFile.Append(".asm");
            args.Append(assemblerCodeFile);
        }
    }
    if (this->additionalParams.IsValid())
    {
        args.Append(" ");
        args.Append(this->additionalParams);
        args.Append(" ");
    }
    if (extraParams.IsValid())
    {
        args.Append(" ");
        args.Append(extraParams);
        args.Append(" ");
    }
    args.Append("/Fo \"");
    args.Append(dstPath);
    args.Append("\" \"");
    args.Append(resolvedSrcPath);
    args.Append("\" ");

    // launch external compiler tool
    AppLauncher appLauncher;
    appLauncher.SetExecutable(this->toolPath);
    appLauncher.SetWorkingDirectory(this->srcShaderDir);
    appLauncher.SetArguments(args);
    appLauncher.SetNoConsoleWindow(this->quiet);
    if (!appLauncher.LaunchWait())
    {
        n_printf("WARNING: failed to launch compiler tool '%s'!\n", this->toolPath.AsCharPtr());
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
/**
    Compile a single shader in Nebula2 legacy mode. This method will
    call the CompileShaderWin360 method with extra arguments.
*/
bool
ShaderCompiler::CompileShaderNebula2(const Util::String& srcFile, const Util::String& dstFile)
{
    n_assert(Platform::Nebula2 == this->platform);
    bool success = true;

    Array<KeyValuePair<String, String>> shaderModels;
    shaderModels.Append(KeyValuePair<String,String>("vs_2_0", "ps_2_0"));
    shaderModels.Append(KeyValuePair<String,String>("vs_2_a", "ps_2_a"));
    shaderModels.Append(KeyValuePair<String,String>("vs_2_0", "ps_2_b"));
    shaderModels.Append(KeyValuePair<String,String>("vs_3_0", "ps_3_0"));

    Array<String> variations;
    variations.Append("");
    variations.Append("dir");
    variations.Append("point");
    variations.Append("dirshadow");
    variations.Append("pointshadow");

    String extraParams;
    IndexT smIndex;
    for (smIndex = 0; smIndex < shaderModels.Size(); smIndex++)
    {
        const String& vsModel = shaderModels[smIndex].Key();
        const String& psModel = shaderModels[smIndex].Value();
        IndexT varIndex;
        for (varIndex = 0; varIndex < variations.Size(); varIndex++)
        {
            const String& var = variations[varIndex];

            // build extra params
            extraParams.Append(" /D VS_PROFILE=");
            extraParams.Append(vsModel);
            extraParams.Append(" /D PS_PROFILE=");
            extraParams.Append(psModel);
            if (psModel == "ps_2_0")
            {
                extraParams.Append(" /D LOWDETAIL=1");
            }
            if (var == "point")
            {
                extraParams.Append(" /D POINTLIGHT");
            }
            else if (var == "dir")
            {
                extraParams.Append(" /D DIRLIGHT");
            }
            else if (var == "pointshadow")
            {
                extraParams.Append(" /D POINTLIGHT /D SHADOW");
            }
            else if (var == "dirshadow")
            {
                extraParams.Append(" /D DIRLIGHT /D SHADOW");
            }
            else
            {
                extraParams.Append(" /D NOLIGHT");
            }

            // build dst shader name
            String n2DstFile = dstFile;
            if (var.IsValid())
            {
                n2DstFile.Append("_");
                n2DstFile.Append(var);
            }
            n2DstFile.Append("_");
            n2DstFile.Append(vsModel);
            n2DstFile.Append("_");
            n2DstFile.Append(psModel);

            // call actual shader compilation method
            success &= this->CompileShaderWin360(srcFile, n2DstFile, extraParams);
        }
    }
    return success;
}

//------------------------------------------------------------------------------
/**
    Compile a single shader for the PS3 platform. On PS3, a shader actually
    consists of a single .xml file, and several .cg program files.
*/
bool
ShaderCompiler::CompileCgShaderPS3(const String& srcFile, const String& dstFile, const String& defines, bool isFragmentShader)
{
    n_assert(Platform::PS3 == this->platform);
    n_assert(srcFile.IsValid());
    n_assert(dstFile.IsValid());
    IoServer* ioServer = IoServer::Instance();

    // build the source and target path for the XML file
    String srcPath;
    srcPath.Format("%s/%s", this->srcShaderDir.AsCharPtr(), srcFile.AsCharPtr());
    String resolvedSrcPath = AssignRegistry::Instance()->ResolveAssignsInString(srcPath);
    String dstPath;
    dstPath.Format("%s/cg/%s", this->dstShaderDir.AsCharPtr(), dstFile.AsCharPtr());
    dstPath.StripFileExtension();
    String resolvedDstPath = AssignRegistry::Instance()->ResolveAssignsInString(dstPath);

    // check if file needs recompilation
    bool compile = this->CheckRecompile(srcPath, dstPath);
    if (!compile)
    {
        return true;
    }

    // split defines
    Array<String> defineTokens = defines.Tokenize("; ");

    // build arguments for cg compiler
    String args;
    args.Append("-p ");
    if (isFragmentShader)
    {
        args.Append("sce_fp_rsx ");
    }
    else
    {
        args.Append("sce_vp_rsx ");
    }
    if (this->debug)
    {                                                                               
        //fkeep-unused: Do not remove unused bindings from the parameter table.
        //debug: Enable debug()calls and output shader debug info.  
        //capture: Instructs the compiler to capture compilation information for shader debugging. 
        args.Append("--fkeep-unused --debug --capture --capture-dir ");    
        // store captured shader compilation in intermediate directory
        args.Append(AssignRegistry::Instance()->ResolveAssigns(IO::URI("int:")).GetTail() + " ");
    }
    if (this->additionalParams.IsValid())
    {
        args.Append(" ");
        args.Append(this->additionalParams);
        args.Append(" ");
    }
    IndexT i;
    for (i = 0; i < defineTokens.Size(); i++)
    {
        args.Append("-D ");
        args.Append(defineTokens[i]);
        args.Append(" ");
    }
    args.Append("-o ");
    args.Append(resolvedDstPath);
    args.Append(" ");
    args.Append(resolvedSrcPath);

    // launch external compiler tool
    AppLauncher appLauncher;
    appLauncher.SetExecutable(this->toolPath);
    appLauncher.SetWorkingDirectory(this->srcShaderDir);
    appLauncher.SetArguments(args);
    appLauncher.SetNoConsoleWindow(this->quiet);
    if (!appLauncher.LaunchWait())
    {
        n_printf("WARNING: failed to launch compiler tool '%s'!\n", this->toolPath.AsCharPtr());
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
/**
    NOTE: Frame shaders are currently simply copied, there is no 
    binary format for them.
*/
bool
ShaderCompiler::CompileFrameShaders()
{
    n_assert(this->srcFrameShaderDir.IsValid());
    n_assert(this->dstFrameShaderDir.IsValid());
    IoServer* ioServer = IoServer::Instance();

    // check if source dir exists
    if (!ioServer->DirectoryExists(this->srcFrameShaderDir))
    {
        n_printf("WARNING: frame shader source directory '%s' not found!\n", this->srcFrameShaderDir.AsCharPtr());
        return false;
    }

    // make sure target dir exists
    ioServer->CreateDirectory(this->dstFrameShaderDir);

    // for each frame shader...
    bool success = true;
    Array<String> srcFiles = ioServer->ListFiles(this->srcFrameShaderDir, "*.xml");
    IndexT i;
    for (i = 0; i < srcFiles.Size(); i++)
    {
        // build absolute source and target filenames
        String srcPath;
        srcPath.Format("%s/%s", this->srcFrameShaderDir.AsCharPtr(), srcFiles[i].AsCharPtr());
        String dstPath;
        dstPath.Format("%s/%s", this->dstFrameShaderDir.AsCharPtr(), srcFiles[i].AsCharPtr());
        success &= ioServer->CopyFile(srcPath, dstPath);
        n_printf("copy frame shader: %s\n", srcPath.AsCharPtr());
    }
    return success;
}

//------------------------------------------------------------------------------
/**
    The shader dictionary is a simple text file with the name of all shaders.
    This lets the shader server load shaders without doing a ListFiles().
*/
bool
ShaderCompiler::WriteShaderDictionary()
{
    String filename;
    filename.Format("%s/shaders.dic", this->dstShaderDir.AsCharPtr());
    Ptr<Stream> stream = IoServer::Instance()->CreateStream(filename);
    Ptr<TextWriter> textWriter = TextWriter::Create();
    textWriter->SetStream(stream);
    if (textWriter->Open())
    {
        String prefix = "shd:";
        IndexT i;
        for (i = 0; i < this->shaderNames.Size(); i++)
        {
            textWriter->WriteLine(prefix + this->shaderNames[i]);        
        }
        textWriter->Close();
        return true;
    }
    return false;
}

} // namespace ToolkitUtil