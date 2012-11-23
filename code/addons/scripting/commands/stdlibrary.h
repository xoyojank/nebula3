#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "scripting/command.h"

//------------------------------------------------------------------------------
namespace Commands
{
class StdLibrary
{
public:
    /// register commands
    static void Register();
};
//------------------------------------------------------------------------------
class ListCmds : public Scripting::Command
{
    __DeclareClass(ListCmds);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    void Callback();
};
//------------------------------------------------------------------------------
class Help : public Scripting::Command
{
    __DeclareClass(Help);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    void Callback(const Util::String& cmdName);
};
} // namespace Commands
//------------------------------------------------------------------------------
