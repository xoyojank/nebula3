#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "scripting/command.h"
#include "io/ioserver.h"
#include "io/assign.h"
#include "io/console.h"
#include "io/assignregistry.h"

//------------------------------------------------------------------------------
namespace Commands
{
class IOLibrary
{
public:
    /// register commands
    static void Register();
};
//------------------------------------------------------------------------------
class SetAssign : public Scripting::Command
{
    __DeclareClass(SetAssign);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    void Callback(const Util::String& assign, const Util::String& path);
};
//------------------------------------------------------------------------------
class GetAssign : public Scripting::Command
{
    __DeclareClass(GetAssign);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    Util::String Callback(const Util::String& assign);
};
//------------------------------------------------------------------------------
class HasAssign : public Scripting::Command
{
    __DeclareClass(HasAssign);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    bool Callback(const Util::String& assign);
};
//------------------------------------------------------------------------------
class ClearAssign : public Scripting::Command
{
    __DeclareClass(ClearAssign);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    void Callback(const Util::String& assign);
};
//------------------------------------------------------------------------------
class ResolveAssigns : public Scripting::Command
{
    __DeclareClass(ResolveAssigns);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    Util::String Callback(const Util::String& path);
};
//------------------------------------------------------------------------------
class CreateDirectory : public Scripting::Command
{
    __DeclareClass(CreateDirectory);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    bool Callback(const Util::String& path);
};
//------------------------------------------------------------------------------
class CopyFile : public Scripting::Command
{
    __DeclareClass(CopyFile);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    bool Callback(const Util::String& sourceFile, const Util::String& targetFile);
};
//------------------------------------------------------------------------------
class DeleteFile : public Scripting::Command
{
    __DeclareClass(DeleteFile);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    bool Callback(const Util::String& fileName);
};
//------------------------------------------------------------------------------
class DeleteDirectory : public Scripting::Command
{
    __DeclareClass(DeleteDirectory);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    bool Callback(const Util::String& path);
};
//------------------------------------------------------------------------------
class FileExists : public Scripting::Command
{
    __DeclareClass(FileExists);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    bool Callback(const Util::String& file);
};
//------------------------------------------------------------------------------
class ListFiles : public Scripting::Command
{
    __DeclareClass(ListFiles);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    Util::Array<Util::String> Callback(const Util::String& path, const Util::String& pattern);
};
//------------------------------------------------------------------------------
class ListDirectories : public Scripting::Command
{
    __DeclareClass(ListDirectories);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    Util::Array<Util::String> Callback(const Util::String& path, const Util::String& pattern);
};
//------------------------------------------------------------------------------
class Print : public Scripting::Command
{
    __DeclareClass(Print);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    void Callback(const Util::String& str);
};
} // namespace Commands
//------------------------------------------------------------------------------
