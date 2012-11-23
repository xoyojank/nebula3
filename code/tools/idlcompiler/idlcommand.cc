//------------------------------------------------------------------------------
//  idlcommand.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/idlcompiler/idlcommand.h"

namespace Tools
{
__ImplementClass(Tools::IDLCommand, 'ILCM', Core::RefCounted);

using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
IDLCommand::IDLCommand()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
IDLCommand::Parse(XmlReader* reader)
{
    n_assert(0 != reader);
    n_assert(reader->GetCurrentNodeName() == "Command");

    // parse attributes
    this->name = reader->GetString("name");
    this->desc = reader->GetString("desc");
    this->fourcc = reader->GetString("fourcc");

    // parse input args
    if (reader->SetToFirstChild("InArg")) do
    {
        Ptr<IDLArg> arg = IDLArg::Create();
        if (!arg->Parse(reader))
        {
            this->SetError(arg->GetError());
            return false;
        }
        this->inArgs.Append(arg);
    }
    while (reader->SetToNextChild("InArg"));

    // parse output args
    if (reader->SetToFirstChild("OutArg")) do
    {
        Ptr<IDLArg> arg = IDLArg::Create();
        if (!arg->Parse(reader))
        {
            this->SetError(arg->GetError());
            return false;
        }
        this->outArgs.Append(arg);
    }
    while (reader->SetToNextChild("OutArg"));

    // read source code fragment
    bool success = reader->SetToFirstChild("Code");
    n_assert2(success, "<Code> element expected!");
    this->code = reader->GetContent();
    reader->SetToParent();

    return true;
}

} // namespace Tools
