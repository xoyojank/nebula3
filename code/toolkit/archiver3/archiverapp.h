#pragma once
//------------------------------------------------------------------------------
/**
    @class Toolkit::ArchiverApp

    Application class for archiver3 tool.

    (C) 2009 Radon Labs GmbH
*/
#include "toolkitutil/toolkitapp.h"

//------------------------------------------------------------------------------
namespace Toolkit
{
class ArchiverApp : public ToolkitUtil::ToolkitApp
{
public:
    /// constructor
    ArchiverApp();
    /// run the application
    virtual void Run();

private:
    /// parse command line arguments
    virtual bool ParseCmdLineArgs();
    /// setup project info object
    virtual bool SetupProjectInfo();
    /// print help text
    virtual void ShowHelp();
    /// pack directory for web deployment
    void PackWebDeploy(const Util::String& dir, const Util::String& webDeployDir);
    /// pack directory using ZIP for the Win32 and Xbox360 platforms
    void PackDirectoryWin360(const Util::String& dir);
    /// pack all asset directories for the Wii platform, and copy to Wii SDK's DVDROOT
    void PackAndCopyWiiArchive();
    /// pack PS3 archive
    void PackPS3Archive();
    /// create PS3 archiver config file
    bool CreatePS3ConfigFile();
    /// recursively pack and copy a web-deployment directory
    void RecursePackWebDeployDirectory(const Util::String& srcDir, const Util::String& dstDir);
    /// compress and copy a file for web deployment
    void CompressCopyFile(const Util::String& srcPath, const Util::String& dstPath);

    Util::String toolPath;
    Util::String wiiDvdRoot;
    Util::Array<Util::String> excludePatterns;
    bool webDeployFlag;
};

} // namespace Toolkit