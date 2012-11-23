#pragma once
#ifndef SCRIPTING_SCRIPTSERVER_H
#define SCRIPTING_SCRIPTSERVER_H
//------------------------------------------------------------------------------
/**
    @class Scripting::ScriptServer

    Server class of the scripting subsystem. The scripting server keeps
    track of all registered class script interfaces and registered
    global script commands. Subclasses of script server know how
    to execute scripts of a specific language.
    
    (C) 2006 Radon Labs
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "io/uri.h"

//------------------------------------------------------------------------------
namespace Scripting
{
class Command;

class ScriptServer : public Core::RefCounted
{
    __DeclareClass(ScriptServer);
    __DeclareSingleton(ScriptServer);
public:
    /// constructor
    ScriptServer();
    /// destructor
    virtual ~ScriptServer();
    /// open the script server
    virtual bool Open();
    /// close the script server
    virtual void Close();
    /// return true if open
    bool IsOpen() const;
    /// register a command with the script server
    virtual void RegisterCommand(const Util::String& name, const Ptr<Command>& cmd);
    /// unregister a command from the script server
    virtual void UnregisterCommand(const Util::String& name);
    /// evaluate a script statement in a string
    virtual bool Eval(const Util::String& str);
    /// evaluate a script file
    virtual bool EvalScript(const IO::URI& uri);
    /// return true if a command has been registered by name
    bool HasCommand(const Util::String& cmdName) const;
    /// return number of registered commands
    SizeT GetNumCommands() const;
    /// return pointer to command at index
    const Ptr<Command>& GetCommandByIndex(IndexT i) const;
    /// get pointer to command by command name
    const Ptr<Command>& GetCommandByName(const Util::String& cmdName) const;
    /// get error string if evaluation failes
    const Util::String& GetError() const;
    /// print all registered commands to stdout
    void PrintCommandList() const;
    /// print help on a command on stdout
    void PrintCommandHelp(const Util::String& cmdName) const;

protected:
    /// clear current error
    void ClearError();
    /// set error string
    void SetError(const Util::String& err);

private:
    bool isOpen;
    Util::String error;
    Util::Dictionary<Util::String, Ptr<Command> > commandRegistry;
};

//------------------------------------------------------------------------------
/**
*/
inline
bool
ScriptServer::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
ScriptServer::SetError(const Util::String& err)
{
    this->error = err;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
ScriptServer::ClearError()
{
    this->error.Clear();
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
ScriptServer::GetError() const
{
    return this->error;
}

} // namespace Scripting
//------------------------------------------------------------------------------
#endif

