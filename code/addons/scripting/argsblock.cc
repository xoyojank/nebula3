//------------------------------------------------------------------------------
//  argsblock.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "scripting/argsblock.h"

namespace Scripting
{
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
ArgsBlock::ArgsBlock()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ArgsBlock::~ArgsBlock()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
ArgsBlock::IsEmpty() const
{
    return this->args.IsEmpty();
}

//------------------------------------------------------------------------------
/**
*/
void
ArgsBlock::Clear()
{
    return this->args.Clear();
}

//------------------------------------------------------------------------------
/**
    Add argument to the args block. This method is usually called
    in the OnRegister() method of a script command object to setup
    the arguments and results block of the command.
*/
void
ArgsBlock::AddArg(const String& name, Arg::Type type)
{
    n_assert(!this->args.Contains(name));
    Arg arg;
    arg.SetType(type);
    this->args.Add(name, arg);
}

//------------------------------------------------------------------------------
/**
*/
SizeT
ArgsBlock::GetNumArgs() const
{
    return this->args.Size();
}

//------------------------------------------------------------------------------
/**
*/
const Util::String&
ArgsBlock::GetArgName(IndexT index) const
{
    return this->args.KeyAtIndex(index);
}

//------------------------------------------------------------------------------
/**
*/
bool
ArgsBlock::HasArg(const String& name) const
{
    return this->args.Contains(name);
}

//------------------------------------------------------------------------------
/**
*/
const Arg&
ArgsBlock::GetArgValue(IndexT index) const
{
    return this->args.ValueAtIndex(index);
}

//------------------------------------------------------------------------------
/**
*/
const Arg&
ArgsBlock::GetArgValue(const Util::String& name) const
{
    return this->args[name];
}

//------------------------------------------------------------------------------
/**
*/
Arg&
ArgsBlock::ArgValue(IndexT index)
{
    return this->args.ValueAtIndex(index);
}

//------------------------------------------------------------------------------
/**
*/
Arg&
ArgsBlock::ArgValue(const Util::String& name)
{
    return this->args[name];
}

} // namespace Scripting
