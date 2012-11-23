//------------------------------------------------------------------------------
//  scriptserver.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "scripting/scriptserver.h"
#include "scripting/command.h"
#include "io/console.h"

namespace Scripting
{
__ImplementClass(Scripting::ScriptServer, 'SCRS', Core::RefCounted);
__ImplementSingleton(Scripting::ScriptServer);

using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
ScriptServer::ScriptServer() :
    isOpen(false)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
ScriptServer::~ScriptServer()
{
    n_assert(!this->isOpen);
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
bool
ScriptServer::Open()
{
    n_assert(!this->isOpen);
    this->isOpen = true;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
ScriptServer::Close()
{
    n_assert(this->isOpen);
    while (!this->commandRegistry.IsEmpty())
    {
        this->UnregisterCommand(this->commandRegistry.KeyAtIndex(0));
    }
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void
ScriptServer::RegisterCommand(const String& name, const Ptr<Command>& cmd)
{
    n_assert(this->IsOpen());
    n_assert(name.IsValid());
    n_assert(cmd.isvalid());
    n_assert(!this->commandRegistry.Contains(name));
    this->commandRegistry.Add(name, cmd);
    cmd->SetName(name);
    cmd->OnRegister();
}

//------------------------------------------------------------------------------
/**
*/
void
ScriptServer::UnregisterCommand(const String& name)
{
    n_assert(this->IsOpen());
    n_assert(name.IsValid());
    n_assert(this->commandRegistry.Contains(name));
    this->commandRegistry[name]->OnUnregister();
    this->commandRegistry.Erase(name);
}

//------------------------------------------------------------------------------
/**
*/
bool
ScriptServer::Eval(const String& /*str*/)
{
    // empty, override in subclass!
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
ScriptServer::EvalScript(const URI& /*uri*/)
{
    // empty, override in subclass!
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
ScriptServer::HasCommand(const Util::String& cmdName) const
{
    return this->commandRegistry.Contains(cmdName);
}

//------------------------------------------------------------------------------
/**
*/
SizeT
ScriptServer::GetNumCommands() const
{
    return this->commandRegistry.Size();
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<Command>&
ScriptServer::GetCommandByIndex(IndexT i) const
{
    return this->commandRegistry.ValueAtIndex(i);
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<Command>&
ScriptServer::GetCommandByName(const Util::String& cmdName) const
{
    return this->commandRegistry[cmdName];
}

//------------------------------------------------------------------------------
/**
*/
void
ScriptServer::PrintCommandList() const
{
    IO::Console* con = IO::Console::Instance();
    IndexT i;
    for (i = 0; i < this->commandRegistry.Size(); i++)
    {
        con->Print("%s\n", this->commandRegistry.ValueAtIndex(i)->GetName().AsCharPtr());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
ScriptServer::PrintCommandHelp(const Util::String& cmdName) const
{
    IO::Console* con = IO::Console::Instance();
    if (this->commandRegistry.Contains(cmdName))
    {
        con->Print("%s\n", this->commandRegistry[cmdName]->GetSyntax().AsCharPtr());
        con->Print("%s\n", this->commandRegistry[cmdName]->GetHelp().AsCharPtr());
    }
    else
    {
        con->Print("Unknown command: %s\n", cmdName.AsCharPtr());
    }
}

} // namespace Scripting