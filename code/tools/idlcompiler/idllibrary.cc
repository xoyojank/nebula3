//------------------------------------------------------------------------------
//  idllibrary.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/idlcompiler/idllibrary.h"

namespace Tools
{
__ImplementClass(Tools::IDLLibrary, 'ILLB', Core::RefCounted);

using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
IDLLibrary::IDLLibrary()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
IDLLibrary::Parse(XmlReader* reader)
{
    n_assert(0 != reader);
    n_assert(reader->GetCurrentNodeName() == "Library");

    // parse attributes
    this->name = reader->GetString("name");
    
    // parse dependency definitions
    if (reader->SetToFirstChild("Dependency")) do
    {
        Ptr<IDLDependency> dep = IDLDependency::Create();
        if (!dep->Parse(reader))
        {
            this->SetError(dep->GetError());
            return false;
        }
        this->dependencies.Append(dep);
    }
    while (reader->SetToNextChild("Dependency"));
    
    // parse command definitions
    if (reader->SetToFirstChild("Command")) do
    {
        Ptr<IDLCommand> cmd = IDLCommand::Create();
        if (!cmd->Parse(reader))
        {
            this->SetError(cmd->GetError());
            return false;
        }
        this->commands.Append(cmd);
    }
    while (reader->SetToNextChild("Command"));
    return true;
}

} // namespace Tools