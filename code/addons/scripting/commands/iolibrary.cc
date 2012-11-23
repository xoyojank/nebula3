//------------------------------------------------------------------------------
//  MACHINE GENERATED, DON'T EDIT!
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "scripting/scriptserver.h"
#include "scripting/arg.h"
#include "iolibrary.h"

__ImplementClass(Commands::SetAssign, 'sasn', Scripting::Command);
__ImplementClass(Commands::GetAssign, 'gasn', Scripting::Command);
__ImplementClass(Commands::HasAssign, 'hasn', Scripting::Command);
__ImplementClass(Commands::ClearAssign, 'casn', Scripting::Command);
__ImplementClass(Commands::ResolveAssigns, 'rpth', Scripting::Command);
__ImplementClass(Commands::CreateDirectory, 'cdir', Scripting::Command);
__ImplementClass(Commands::CopyFile, 'cpfl', Scripting::Command);
__ImplementClass(Commands::DeleteFile, 'dlfl', Scripting::Command);
__ImplementClass(Commands::DeleteDirectory, 'dldr', Scripting::Command);
__ImplementClass(Commands::FileExists, 'fexs', Scripting::Command);
__ImplementClass(Commands::ListFiles, 'lsfl', Scripting::Command);
__ImplementClass(Commands::ListDirectories, 'lsdr', Scripting::Command);
__ImplementClass(Commands::Print, 'prnt', Scripting::Command);

namespace Commands
{
//------------------------------------------------------------------------------
/**
*/
void
IOLibrary::Register()
{
    Scripting::ScriptServer* scriptServer = Scripting::ScriptServer::Instance();
    scriptServer->RegisterCommand("setassign", SetAssign::Create());
    scriptServer->RegisterCommand("getassign", GetAssign::Create());
    scriptServer->RegisterCommand("hasassign", HasAssign::Create());
    scriptServer->RegisterCommand("clearassign", ClearAssign::Create());
    scriptServer->RegisterCommand("resolveassigns", ResolveAssigns::Create());
    scriptServer->RegisterCommand("createdirectory", CreateDirectory::Create());
    scriptServer->RegisterCommand("copyfile", CopyFile::Create());
    scriptServer->RegisterCommand("deletefile", DeleteFile::Create());
    scriptServer->RegisterCommand("deletedirectory", DeleteDirectory::Create());
    scriptServer->RegisterCommand("fileexists", FileExists::Create());
    scriptServer->RegisterCommand("listfiles", ListFiles::Create());
    scriptServer->RegisterCommand("listdirectories", ListDirectories::Create());
    scriptServer->RegisterCommand("print", Print::Create());
}

//------------------------------------------------------------------------------
/**
*/
void
SetAssign::OnRegister()
{
    Scripting::Command::OnRegister();
    this->args.AddArg("assign", Scripting::Arg::String);
    this->args.AddArg("path", Scripting::Arg::String);
}

//------------------------------------------------------------------------------
/**
*/
Util::String
SetAssign::GetHelp() const
{
    return "Define a new filesystem assign.";
}
//------------------------------------------------------------------------------
/**
*/
bool
SetAssign::OnExecute()
{
    const Util::String& assign = this->args.GetArgValue(0).GetString();
    const Util::String& path = this->args.GetArgValue(1).GetString();
    this->Callback(assign, path);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void SetAssign::Callback(const Util::String& assign, const Util::String& path)
{
IO::AssignRegistry::Instance()->SetAssign(IO::Assign(assign, path));
}

//------------------------------------------------------------------------------
/**
*/
void
GetAssign::OnRegister()
{
    Scripting::Command::OnRegister();
    this->args.AddArg("assign", Scripting::Arg::String);
    this->results.AddArg("path", Scripting::Arg::String);
}

//------------------------------------------------------------------------------
/**
*/
Util::String
GetAssign::GetHelp() const
{
    return "Get a filesystem assign.";
}
//------------------------------------------------------------------------------
/**
*/
bool
GetAssign::OnExecute()
{
    const Util::String& assign = this->args.GetArgValue(0).GetString();
    Util::String path = this->Callback(assign);
    this->results.ArgValue(0).SetString(path);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
Util::String GetAssign::Callback(const Util::String& assign)
{
return IO::AssignRegistry::Instance()->GetAssign(assign);
}

//------------------------------------------------------------------------------
/**
*/
void
HasAssign::OnRegister()
{
    Scripting::Command::OnRegister();
    this->args.AddArg("assign", Scripting::Arg::String);
    this->results.AddArg("exists", Scripting::Arg::Bool);
}

//------------------------------------------------------------------------------
/**
*/
Util::String
HasAssign::GetHelp() const
{
    return "Return true if an assign exists.";
}
//------------------------------------------------------------------------------
/**
*/
bool
HasAssign::OnExecute()
{
    const Util::String& assign = this->args.GetArgValue(0).GetString();
    bool exists = this->Callback(assign);
    this->results.ArgValue(0).SetBool(exists);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool HasAssign::Callback(const Util::String& assign)
{
return IO::AssignRegistry::Instance()->HasAssign(assign);
}

//------------------------------------------------------------------------------
/**
*/
void
ClearAssign::OnRegister()
{
    Scripting::Command::OnRegister();
    this->args.AddArg("assign", Scripting::Arg::String);
}

//------------------------------------------------------------------------------
/**
*/
Util::String
ClearAssign::GetHelp() const
{
    return "Clears an existing assign.";
}
//------------------------------------------------------------------------------
/**
*/
bool
ClearAssign::OnExecute()
{
    const Util::String& assign = this->args.GetArgValue(0).GetString();
    this->Callback(assign);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void ClearAssign::Callback(const Util::String& assign)
{
IO::AssignRegistry::Instance()->ClearAssign(assign);
}

//------------------------------------------------------------------------------
/**
*/
void
ResolveAssigns::OnRegister()
{
    Scripting::Command::OnRegister();
    this->args.AddArg("path", Scripting::Arg::String);
    this->results.AddArg("nativePath", Scripting::Arg::String);
}

//------------------------------------------------------------------------------
/**
*/
Util::String
ResolveAssigns::GetHelp() const
{
    return "Resolve a path with assigns into a native path.";
}
//------------------------------------------------------------------------------
/**
*/
bool
ResolveAssigns::OnExecute()
{
    const Util::String& path = this->args.GetArgValue(0).GetString();
    Util::String nativePath = this->Callback(path);
    this->results.ArgValue(0).SetString(nativePath);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
Util::String ResolveAssigns::Callback(const Util::String& path)
{
return IO::AssignRegistry::Instance()->ResolveAssigns(path).AsString();
}

//------------------------------------------------------------------------------
/**
*/
void
CreateDirectory::OnRegister()
{
    Scripting::Command::OnRegister();
    this->args.AddArg("path", Scripting::Arg::String);
    this->results.AddArg("result", Scripting::Arg::Bool);
}

//------------------------------------------------------------------------------
/**
*/
Util::String
CreateDirectory::GetHelp() const
{
    return "Create a directory.";
}
//------------------------------------------------------------------------------
/**
*/
bool
CreateDirectory::OnExecute()
{
    const Util::String& path = this->args.GetArgValue(0).GetString();
    bool result = this->Callback(path);
    this->results.ArgValue(0).SetBool(result);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool CreateDirectory::Callback(const Util::String& path)
{
return IO::IoServer::Instance()->CreateDirectory(path);
}

//------------------------------------------------------------------------------
/**
*/
void
CopyFile::OnRegister()
{
    Scripting::Command::OnRegister();
    this->args.AddArg("sourceFile", Scripting::Arg::String);
    this->args.AddArg("targetFile", Scripting::Arg::String);
    this->results.AddArg("success", Scripting::Arg::Bool);
}

//------------------------------------------------------------------------------
/**
*/
Util::String
CopyFile::GetHelp() const
{
    return "Copy a source file to a target file.";
}
//------------------------------------------------------------------------------
/**
*/
bool
CopyFile::OnExecute()
{
    const Util::String& sourceFile = this->args.GetArgValue(0).GetString();
    const Util::String& targetFile = this->args.GetArgValue(1).GetString();
    bool success = this->Callback(sourceFile, targetFile);
    this->results.ArgValue(0).SetBool(success);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool CopyFile::Callback(const Util::String& sourceFile, const Util::String& targetFile)
{
return IO::IoServer::Instance()->CopyFile(sourceFile, targetFile);
}

//------------------------------------------------------------------------------
/**
*/
void
DeleteFile::OnRegister()
{
    Scripting::Command::OnRegister();
    this->args.AddArg("fileName", Scripting::Arg::String);
    this->results.AddArg("success", Scripting::Arg::Bool);
}

//------------------------------------------------------------------------------
/**
*/
Util::String
DeleteFile::GetHelp() const
{
    return "Delete a file.";
}
//------------------------------------------------------------------------------
/**
*/
bool
DeleteFile::OnExecute()
{
    const Util::String& fileName = this->args.GetArgValue(0).GetString();
    bool success = this->Callback(fileName);
    this->results.ArgValue(0).SetBool(success);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DeleteFile::Callback(const Util::String& fileName)
{
return IO::IoServer::Instance()->DeleteFile(fileName);
}

//------------------------------------------------------------------------------
/**
*/
void
DeleteDirectory::OnRegister()
{
    Scripting::Command::OnRegister();
    this->args.AddArg("path", Scripting::Arg::String);
    this->results.AddArg("success", Scripting::Arg::Bool);
}

//------------------------------------------------------------------------------
/**
*/
Util::String
DeleteDirectory::GetHelp() const
{
    return "Delete a directory.";
}
//------------------------------------------------------------------------------
/**
*/
bool
DeleteDirectory::OnExecute()
{
    const Util::String& path = this->args.GetArgValue(0).GetString();
    bool success = this->Callback(path);
    this->results.ArgValue(0).SetBool(success);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DeleteDirectory::Callback(const Util::String& path)
{
return IO::IoServer::Instance()->DeleteDirectory(path);
}

//------------------------------------------------------------------------------
/**
*/
void
FileExists::OnRegister()
{
    Scripting::Command::OnRegister();
    this->args.AddArg("file", Scripting::Arg::String);
    this->results.AddArg("exists", Scripting::Arg::Bool);
}

//------------------------------------------------------------------------------
/**
*/
Util::String
FileExists::GetHelp() const
{
    return "Check if a file exists.";
}
//------------------------------------------------------------------------------
/**
*/
bool
FileExists::OnExecute()
{
    const Util::String& file = this->args.GetArgValue(0).GetString();
    bool exists = this->Callback(file);
    this->results.ArgValue(0).SetBool(exists);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool FileExists::Callback(const Util::String& file)
{
return IO::IoServer::Instance()->FileExists(file);
}

//------------------------------------------------------------------------------
/**
*/
void
ListFiles::OnRegister()
{
    Scripting::Command::OnRegister();
    this->args.AddArg("path", Scripting::Arg::String);
    this->args.AddArg("pattern", Scripting::Arg::String);
    this->results.AddArg("result", Scripting::Arg::StringArray);
}

//------------------------------------------------------------------------------
/**
*/
Util::String
ListFiles::GetHelp() const
{
    return "List files in a directory.";
}
//------------------------------------------------------------------------------
/**
*/
bool
ListFiles::OnExecute()
{
    const Util::String& path = this->args.GetArgValue(0).GetString();
    const Util::String& pattern = this->args.GetArgValue(1).GetString();
    Util::Array<Util::String> result = this->Callback(path, pattern);
    this->results.ArgValue(0).SetStringArray(result);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
Util::Array<Util::String> ListFiles::Callback(const Util::String& path, const Util::String& pattern)
{
return IO::IoServer::Instance()->ListFiles(path, pattern);
}

//------------------------------------------------------------------------------
/**
*/
void
ListDirectories::OnRegister()
{
    Scripting::Command::OnRegister();
    this->args.AddArg("path", Scripting::Arg::String);
    this->args.AddArg("pattern", Scripting::Arg::String);
    this->results.AddArg("result", Scripting::Arg::StringArray);
}

//------------------------------------------------------------------------------
/**
*/
Util::String
ListDirectories::GetHelp() const
{
    return "List directories in a directory.";
}
//------------------------------------------------------------------------------
/**
*/
bool
ListDirectories::OnExecute()
{
    const Util::String& path = this->args.GetArgValue(0).GetString();
    const Util::String& pattern = this->args.GetArgValue(1).GetString();
    Util::Array<Util::String> result = this->Callback(path, pattern);
    this->results.ArgValue(0).SetStringArray(result);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
Util::Array<Util::String> ListDirectories::Callback(const Util::String& path, const Util::String& pattern)
{
return IO::IoServer::Instance()->ListDirectories(path, pattern);
}

//------------------------------------------------------------------------------
/**
*/
void
Print::OnRegister()
{
    Scripting::Command::OnRegister();
    this->args.AddArg("str", Scripting::Arg::String);
}

//------------------------------------------------------------------------------
/**
*/
Util::String
Print::GetHelp() const
{
    return "Output string to stdout.";
}
//------------------------------------------------------------------------------
/**
*/
bool
Print::OnExecute()
{
    const Util::String& str = this->args.GetArgValue(0).GetString();
    this->Callback(str);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void Print::Callback(const Util::String& str)
{
IO::Console::Instance()->Print("%s\n", str.AsCharPtr());
}

} // namespace Commands
//------------------------------------------------------------------------------
