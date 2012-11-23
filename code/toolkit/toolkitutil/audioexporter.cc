//------------------------------------------------------------------------------
//  audioexporter.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "audioexporter.h"
#include "io/ioserver.h"
#include "io/uri.h"
#include "toolkitutil/applauncher.h"

namespace ToolkitUtil
{
using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
AudioExporter::AudioExporter() :
    platform(Platform::Win32)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
AudioExporter::~AudioExporter()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
AudioExporter::Export()
{
    if ((this->platform == Platform::Win32) || (this->platform == Platform::Xbox360))
    {
        return this->ExportFmod();
    }
    else if (this->platform == Platform::PS3)
    {
        return this->ExportFmod();
    }
	else if (this->platform == Platform::Wii)
    {
        return this->ExportWii();
    }
    else
    {
        n_printf("WARNING: unsupported platform '%s' in AudioExporter!\n", Platform::ToString(this->platform).AsCharPtr());
        return true;
    }
}

//------------------------------------------------------------------------------
/**
    Audio export function for Windows and Xbox360 platform.
*/
bool
AudioExporter::ExportXact()
{
    n_assert(this->toolPath.IsValid());
    n_assert(this->projectFile.IsValid());      
    n_assert(this->dstDir.IsValid());
    IoServer* ioServer = IoServer::Instance();

    // resolve assigns in project file path
    String resolvedProjFilePath = AssignRegistry::Instance()->ResolveAssignsInString(this->projectFile);
    if (!ioServer->FileExists(resolvedProjFilePath))
    {
        n_printf("WARNING: XACT project file '%s' does not exist!\n", resolvedProjFilePath.AsCharPtr());
        return false;
    }
    
    // resolve assigns in destination dir and make sure that path exists
    String resolvedDstPath = AssignRegistry::Instance()->ResolveAssignsInString(this->dstDir);
    ioServer->CreateDirectory(this->dstDir);

    // setup command line args and launch xactbld3.exe
    String args = "/L /X:HEADER /X:CUELIST /X:REPORT ";
    if (this->force)
    {
        args.Append("/F ");
    }
    if (Platform::Win32 == this->platform)
    {
        args.Append("/WIN32 ");
    }
    else
    {
        args.Append("/XBOX360 ");
    }
    args.Append("\"");
    args.Append(resolvedProjFilePath);
    args.Append("\" \"");
    args.Append(resolvedDstPath);
    args.Append("\"");

    AppLauncher appLauncher;
    appLauncher.SetExecutable(this->toolPath);
    appLauncher.SetWorkingDirectory(resolvedDstPath);
    appLauncher.SetArguments(args);
    if (!appLauncher.LaunchWait())
    {
        n_printf("WARNING: failed to launch audio tool '%s'!\n", this->toolPath.AsCharPtr());
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
/**
    Audio export function for Wii platform.
*/
bool
AudioExporter::ExportWii()
{
	n_assert(this->toolPath.IsValid());
	n_assert(this->srcDir.IsValid());
    n_assert(this->dstDir.IsValid());

	IoServer* ioServer = IoServer::Instance();

	// find all SoundMaker projectfiles in the audio folder
	this->FindRspjFiles(this->srcDir);
	
	// convert all SoundMaker projectfiles (".rspj") to SoundArchives (".brsar")
	AppLauncher convAppLauncher;
    convAppLauncher.SetExecutable(this->toolPath);
	convAppLauncher.SetWorkingDirectory("proj:work");
	IndexT i;
    for (i = 0; i < this->rspjFiles.Size(); i++)
    {
		convAppLauncher.SetArguments(AssignRegistry::Instance()->ResolveAssignsInString(this->rspjFiles[i]));
		if (!convAppLauncher.LaunchWait())
		{
			n_error("Failed to launch '%s %s'!\n", this->toolPath.AsCharPtr(), this->rspjFiles[i].AsCharPtr());
			return false;
		}
    }

	// copy converted files to destination
	String localDstPath = AssignRegistry::Instance()->ResolveAssignsInString(this->dstDir);
	for (i = 0; i < this->rspjFiles.Size(); i++)
    {
		// create destination folder
		String destFolder = this->rspjFiles[i];
		destFolder.SubstituteString("src:audio", "");
		destFolder.TerminateAtIndex(destFolder.FindStringIndex("/rspj"));
		destFolder = localDstPath + destFolder;
		if (!ioServer->DirectoryExists(destFolder))
		{
			ioServer->CreateDirectory(destFolder);
		}

		// copy files from source folder to destination folder
		String srcFolder = AssignRegistry::Instance()->ResolveAssignsInString(this->srcFolders[i]);
		this->CopyDirectory(srcFolder, destFolder);
    }
	return true;
}

//------------------------------------------------------------------------------
/**
*/
void
AudioExporter::FindRspjFiles(String folder)
{
	IoServer* ioServer = IoServer::Instance();

	// look for rspj files
	Array<String> rspjFiles = ioServer->ListFiles(folder, "*.rspj");
	IndexT i;
    for (i = 0; i < rspjFiles.Size(); i++)
    {
		this->rspjFiles.Append((folder + "/" + rspjFiles[i]));
		this->srcFolders.Append(folder + "/" + "output/dvddata");
    }

	// step through all subfolders
	Array<String> subfolders = ioServer->ListDirectories(folder, "*");
	for (i = 0; i < subfolders.Size(); i++)
    {
		if ((subfolders[i] != "CVS") && (subfolders[i] != ".svn"))
        {
			this->FindRspjFiles((folder + "/" + subfolders[i]));
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
AudioExporter::CopyDirectory(Util::String src, Util::String dst)
{	
	IoServer* ioServer = IoServer::Instance();

	// copy all files in the directory
	Array<String> files = ioServer->ListFiles(src, "*");
	IndexT i;
    for (i = 0; i < files.Size(); i++)
    {
		ioServer->CopyFile(src + "/" + files[i], dst + "/" + files[i]);
    }

	// copy all subdirectories
	Array<String> directories = ioServer->ListDirectories(src, "*");
	for (i = 0; i < directories.Size(); i++)
    {
		ioServer->CreateDirectory(dst + "/" + directories[i]);
		this->CopyDirectory(src + "/" + directories[i], dst + "/" + directories[i]);
    }
}

//------------------------------------------------------------------------------
/**
    Audio export function for fmod.
*/
bool
AudioExporter::ExportFmod()
{
    n_assert(this->toolPath.IsValid());
    n_assert(this->srcDir.IsValid());
    n_assert(this->dstDir.IsValid());
    IoServer* ioServer = IoServer::Instance();

    // we must ignore -force, since fmod has no incremental build yet
    // http://www.fmod.org/forum/viewtopic.php?t=7144&highlight=fmoddesignercl&sid=fb81747c1268475a82a1ed1b99f8d38e
    // doing it ourselves would be overkill, since we have to look in the projectfile,
    // search for raw source files like waves etc in there, search in which bank it is, compare
    // banks build-time with wav's build time, quite error prone

    // 1. create audio export directory
    // resolve assigns in destination dir and make sure that path exists
    const String resolvedDstPath = AssignRegistry::Instance()->ResolveAssignsInString(this->dstDir);
    const String resolvedSrcPath = AssignRegistry::Instance()->ResolveAssignsInString(this->srcDir);
    ioServer->CreateDirectory(this->dstDir);

    // 2. collect all *.fdp files (fmod event project file)
    Util::Array<Util::String> projects;
    this->FmodFindProjectFiles(resolvedSrcPath, projects);

    Util::String baseArgs;
    switch(this->platform)
    {
        case Platform::Win32:
            baseArgs ="-pc ";
            break;
        case Platform::Xbox360:
            baseArgs ="-xbox360 ";
            break;
        case Platform::Wii:
            baseArgs ="-wii ";
            break;
        case Platform::PS3:
            baseArgs ="-ps3 ";
            break;
        default:
            n_error("platform not supported\n");
            break;
    }
    baseArgs += "-b " + resolvedDstPath + "/";
    // 3. export each project seperately
    int index;
    String projectName;
    for(index = 0; index < projects.Size(); ++index)
    {
        projectName = projects[index].ExtractFileName();
        projectName.StripFileExtension();
        // create audio/project - directory
        ioServer->CreateDirectory(this->dstDir + "/" + projectName);

        String args = baseArgs + projectName + " -r " + projects[index];
        AppLauncher appLauncher;
        appLauncher.SetExecutable(this->toolPath);
        appLauncher.SetWorkingDirectory(resolvedDstPath);
        appLauncher.SetArguments(args);
        if (!appLauncher.LaunchWait())
        {
            n_printf("WARNING: failed to launch audio tool '%s'!\n", this->toolPath.AsCharPtr());
            return false;
        }

        //////////////////////////////////////
        // delete files we done need anymore

        // delete folder ".fsbcache" and its files
        int i;
        Util::Array<Util::String> files;
        const String projectFolder = resolvedDstPath + "/" + projectName;
        {
            const String projectCacheFolder = projectFolder + "/.fsbcache";
            files = ioServer->ListFiles(projectCacheFolder, "*");
            for(i = 0; i < files.Size(); ++i)
            {
                ioServer->DeleteFile(projectCacheFolder + "/" + files[i]);
            }
            files.Clear();
            ioServer->DeleteDirectory(projectCacheFolder);
        }

        // delete *.seq files
        files = ioServer->ListFiles(projectFolder, "*.seq");
        for(i = 0; i < files.Size(); ++i)
        {
            ioServer->DeleteFile(projectFolder + "/" + files[i]);
        }
        files.Clear();
    }
    // delete log-file
    ioServer->DeleteFile(resolvedDstPath + "/fmod_designer.log");

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
AudioExporter::FmodFindProjectFiles(const Util::String &folder, Util::Array<Util::String> &projectFiles) const
{
	IoServer* ioServer = IoServer::Instance();
	// look for fmod project files in folder
	projectFiles = ioServer->ListFiles(folder, "*.fdp");
	IndexT i;
    for (i = 0; i < projectFiles.Size(); i++)
    {
        projectFiles[i] = folder + "/" + projectFiles[i];
    }
}

} // namespace ToolkitUtil
