//------------------------------------------------------------------------------
//  luaserver.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "scripting/lua/luaserver.h"
#include "scripting/command.h"
#include "scripting/argsblock.h"
#include "io/console.h"
#include "lua/lualib.h"

namespace Scripting
{
__ImplementClass(Scripting::LuaServer, 'LUAS', Scripting::ScriptServer);
__ImplementSingleton(Scripting::LuaServer);

using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
LuaServer::LuaServer() :
    luaState(0)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
LuaServer::~LuaServer()
{
    if (this->IsOpen())
    {
        this->Close();
    }
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
    The custom memory allocator function for LUA.
*/
void*
LuaServer::LuaAllocator(void* /*ud*/, void* ptr, size_t osize, size_t nsize)
{
    if (0 == nsize)
    {
        // free a block of memory
        if (0 != ptr)
        {
            Memory::Free(Memory::DefaultHeap, ptr);
        }
        return 0;
    }
    else
    {
        if (0 == osize)
        {
            // allocate a block of memory
            return Memory::Alloc(Memory::DefaultHeap, nsize);
        }
        else
        {
            // reallocate a block of memory
            return Memory::Realloc(Memory::DefaultHeap, ptr, nsize);
        }
    }
}

//------------------------------------------------------------------------------
/**
    Callback function for lua_load() to load Lua code from a String object.
*/
const char*
LuaServer::LuaStringReader(lua_State* /*s*/, LuaStringReaderData* data, size_t* size)
{
    n_assert(0 != data);
    if (0 != data->str)
    {
        *size = data->str->Length();
        const char* code = data->str->AsCharPtr();
        data->str = 0;
        return code;
    }
    else
    {
        // we have already been called, return end-of-data
        return 0;
    }
}

//------------------------------------------------------------------------------
/**
    This is the global callback function for all custom LUA commands which
    have been created with RegisterCommand().
*/
int
LuaServer::LuaFunctionCallback(lua_State* s)
{
    n_assert(0 != s);
    LuaServer* luaServer = LuaServer::Instance();
    IO::Console* con = IO::Console::Instance();

    // get the associated scripting command object
    Command* cmd = (Command*) lua_touserdata(luaServer->luaState, lua_upvalueindex(1));

    // pop arguments from stack
    ArgsBlock& cmdArgs = cmd->Arguments();
    SizeT numLuaArgs = lua_gettop(luaServer->luaState);
    if (numLuaArgs == cmdArgs.GetNumArgs())
    {
        IndexT argIndex;
        for (argIndex = 0; argIndex < cmdArgs.GetNumArgs(); argIndex++)
        {
            // Lua arg indices are 1-based
            Arg& curArg = cmdArgs.ArgValue(argIndex);
            switch (curArg.GetType())
            {
                case Arg::Int:
                    curArg.SetInt((int)lua_tonumber(luaServer->luaState, argIndex + 1));
                    break;

                case Arg::Float:
                    curArg.SetFloat((float)lua_tonumber(luaServer->luaState, argIndex + 1));
                    break;

                case Arg::Bool:
                    curArg.SetBool((lua_toboolean(luaServer->luaState, argIndex + 1) == 0) ? false : true);
                    break;

                case Arg::String:
                    curArg.SetString(lua_tostring(luaServer->luaState, argIndex + 1));
                    break;

                default:
                    con->Print("Invalid input type '%s' in cmd '%s'!\n",
                        Arg::TypeToString(curArg.GetType()).AsCharPtr(),
                        cmd->GetName().AsCharPtr());
                    return 0;
            }
        }

        // now execute the command
        cmd->OnExecute();
        
        // transfer results back to LUA
        const ArgsBlock& results = cmd->GetResults();
        IndexT resIndex;
        SizeT numResults = results.GetNumArgs();
        for (resIndex = 0; resIndex < numResults; resIndex++)
        {
            const Arg& curArg = results.GetArgValue(resIndex);
            switch (curArg.GetType())
            {
                case Arg::Int:
                    lua_pushnumber(luaServer->luaState, curArg.GetInt());
                    break;
                case Arg::Float:
                    lua_pushnumber(luaServer->luaState, curArg.GetFloat());
                    break;
                case Arg::Bool:
                    lua_pushboolean(luaServer->luaState, curArg.GetBool());
                    break;
                case Arg::String:
                    lua_pushstring(luaServer->luaState, curArg.GetString().AsCharPtr());
                    break;
                case Arg::IntArray:
                    {
                        const Array<int>& intArray = curArg.GetIntArray();
                        lua_newtable(luaServer->luaState);
                        int tableIndex = lua_gettop(luaServer->luaState);
                        IndexT i;
                        for (i = 0; i < intArray.Size(); i++)
                        {
                            lua_pushnumber(luaServer->luaState, i);
                            lua_pushnumber(luaServer->luaState, intArray[i]);
                            lua_settable(luaServer->luaState, tableIndex);
                        }
                    }
                    break;
                case Arg::FloatArray:
                    {
                        const Array<float>& floatArray = curArg.GetFloatArray();
                        lua_newtable(luaServer->luaState);
                        int tableIndex = lua_gettop(luaServer->luaState);
                        IndexT i;
                        for (i = 0; i < floatArray.Size(); i++)
                        {
                            lua_pushnumber(luaServer->luaState, i);
                            lua_pushnumber(luaServer->luaState, floatArray[i]);
                            lua_settable(luaServer->luaState, tableIndex);
                        }
                    }
                    break;

                case Arg::BoolArray:
                    {
                        const Array<bool>& boolArray = curArg.GetBoolArray();
                        lua_newtable(luaServer->luaState);
                        int tableIndex = lua_gettop(luaServer->luaState);
                        IndexT i;
                        for (i = 0; i < boolArray.Size(); i++)
                        {
                            lua_pushnumber(luaServer->luaState, i);
                            lua_pushboolean(luaServer->luaState, boolArray[i]);
                            lua_settable(luaServer->luaState, tableIndex);
                        }
                    }
                    break;

                case Arg::StringArray:
                    {
                        const Array<String>& stringArray = curArg.GetStringArray();
                        lua_newtable(luaServer->luaState);
                        int tableIndex = lua_gettop(luaServer->luaState);
                        IndexT i;
                        for (i = 0; i < stringArray.Size(); i++)
                        {
                            lua_pushnumber(luaServer->luaState, i);
                            lua_pushstring(luaServer->luaState, stringArray[i].AsCharPtr());
                            lua_settable(luaServer->luaState, tableIndex);
                        }
                    }
                    break;

                default:
                    con->Print("Invalid result type '%s' in cmd '%s'!\n",
                        Arg::TypeToString(curArg.GetType()).AsCharPtr(),
                        cmd->GetName().AsCharPtr());
                    return 0;
            }
        }
        return numResults;
    }
    else
    {
        con->Print("Wrong number of args in '%s'\n", cmd->GetName().AsCharPtr());
        con->Print("Syntax is: %s\n", cmd->GetSyntax().AsCharPtr());
        return 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
LuaServer::Open()
{
    n_assert(!this->IsOpen());
    n_assert(0 == this->luaState);
    if (ScriptServer::Open())
    {
        // initialize Lua
        this->luaState = lua_newstate(LuaAllocator, 0);
        n_assert(0 != this->luaState);

        // provide access to some standard libraries
        luaopen_base(this->luaState);
        luaopen_string(this->luaState);
        luaopen_table(this->luaState);
        luaopen_math(this->luaState);
        luaopen_debug(this->luaState);

        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
LuaServer::Close()
{
    n_assert(this->IsOpen());
    n_assert(0 != this->luaState);
    
    // this will unregister all commands
    ScriptServer::Close();

    // close Lua
    lua_close(this->luaState);
    this->luaState = 0;    
}

//------------------------------------------------------------------------------
/**
    Registers a new script command with the LUA server.
*/
void
LuaServer::RegisterCommand(const String& name, const Ptr<Command>& cmd)
{
    n_assert(0 != this->luaState);

    // call parent class to add command to the registry
    ScriptServer::RegisterCommand(name, cmd);
    
    // Register the command with Lua, all commands are assigned
    // to the same C-callback function. In order to identify which
    // command has called the C-callback we push the pointer
    // to the Command object as a C closure (see Lua ref manual for details)
    lua_pushstring(this->luaState, name.AsCharPtr());
    lua_pushlightuserdata(this->luaState, cmd);
    lua_pushcclosure(this->luaState, LuaFunctionCallback, 1);
    lua_settable(this->luaState, LUA_GLOBALSINDEX);
}

//------------------------------------------------------------------------------
/**
    Unregister a script command.
*/
void
LuaServer::UnregisterCommand(const String& name)
{
    // FIXME: hmm, how do we unregister a command from LUA?

    ScriptServer::UnregisterCommand(name);
}

//------------------------------------------------------------------------------
/**
    Evaluates a piece of LUA code in a string.
*/
bool
LuaServer::Eval(const String& str)
{
    n_assert(this->IsOpen());
    n_assert(0 != this->luaState);
    n_assert(str.IsValid());

    // clear current error string
    this->ClearError();

    // let Lua compile the chunk
    LuaStringReaderData data;
    data.str = &str;
    int res = lua_load(this->luaState, (lua_Reader) LuaStringReader, &data, "LuaServer::Eval()");
    if (0 == res)
    {
        // execute the piece of code
        // NOTE: the error handler is 0, but if we want a
        // stack trace this would be the place to create it
        int res = lua_pcall(this->luaState, 0, 0, 0);
        if (res == 0)
        {
            // all ok
            return true;
        }
        else
        {
            // an error occured during execution, get the error message from the stack
            this->SetError(lua_tostring(this->luaState, lua_gettop(this->luaState)));
            return false;
        }
    }
    else
    {
        // an error occured during compilation, get the error message from the stack
        this->SetError(lua_tostring(this->luaState, lua_gettop(this->luaState)));
        return false;
    }
}
 
} // namespace Scripting