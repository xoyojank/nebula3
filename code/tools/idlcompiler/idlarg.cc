//------------------------------------------------------------------------------
//  idlarg.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/idlcompiler/idlarg.h"  

namespace Tools
{
__ImplementClass(Tools::IDLArg, 'ILAG', Core::RefCounted);

using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
IDLArg::IDLArg()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
IDLArg::Parse(XmlReader* reader)
{
    n_assert(0 != reader);
    this->name = reader->GetString("name");
    this->type = reader->GetString("type");
    this->defaultValue = reader->GetOptString("default", "");
    this->serialize = reader->GetOptBool("serialize", false);
    if (reader->SetToFirstChild("Encode"))
    {
        this->encodeCode = reader->GetContent();
        reader->SetToParent();
    }
    if (reader->SetToFirstChild("Decode"))
    {
        this->decodeCode = reader->GetContent();
        reader->SetToParent();
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
IDLArg::IsValidType(const Util::String& str)
{
    if ((str == "Char")
        ||(str == "String")
        ||(str == "Float")
        ||(str == "Float2")
        ||(str == "Float4")
        ||(str == "Int")
        ||(str == "UInt")
        ||(str == "Bool")
        ||(str == "Matrix44")
        ||(str == "Point")
        ||(str == "Vector")
        ||(str == "Double")
        ||(str == "Short")
        ||(str == "UShort")
        ||(str == "Blob")
        ||(str == "Guid"))
    {
        return true;
    }   

    return false;
}

} // namespace Tools
