//------------------------------------------------------------------------------
//  MACHINE GENERATED, DON'T EDIT!
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "scripting/scriptserver.h"
#include "scripting/arg.h"
#include "stdlibrary.h"

__ImplementClass(Commands::ListCmds, 'LSCD', Scripting::Command);
__ImplementClass(Commands::Help, 'HELP', Scripting::Command);

namespace Commands
{
//------------------------------------------------------------------------------
/**
*/
void
StdLibrary::Register()
{
    Scripting::ScriptServer* scriptServer = Scripting::ScriptServer::Instance();
    scriptServer->RegisterCommand("listcmds", ListCmds::Create());
    scriptServer->RegisterCommand("help", Help::Create());
}

//------------------------------------------------------------------------------
/**
*/
void
ListCmds::OnRegister()
{
    Scripting::Command::OnRegister();
}

//------------------------------------------------------------------------------
/**
*/
Util::String
ListCmds::GetHelp() const
{
    return "List registered script commands.";
}
//------------------------------------------------------------------------------
/**
*/
bool
ListCmds::OnExecute()
{
    this->Callback();
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void ListCmds::Callback()
{
Scripting::ScriptServer::Instance()->PrintCommandList();
}

//------------------------------------------------------------------------------
/**
*/
void
Help::OnRegister()
{
    Scripting::Command::OnRegister();
    this->args.AddArg("cmdName", Scripting::Arg::String);
}

//------------------------------------------------------------------------------
/**
*/
Util::String
Help::GetHelp() const
{
    return "Show help for a command.";
}
//------------------------------------------------------------------------------
/**
*/
bool
Help::OnExecute()
{
    const Util::String& cmdName = this->args.GetArgValue(0).GetString();
    this->Callback(cmdName);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void Help::Callback(const Util::String& cmdName)
{
Scripting::ScriptServer::Instance()->PrintCommandHelp(cmdName);
}

} // namespace Commands
//------------------------------------------------------------------------------
