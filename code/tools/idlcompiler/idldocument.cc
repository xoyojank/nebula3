//------------------------------------------------------------------------------
//  idldocument.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/idlcompiler/idldocument.h"

namespace Tools
{
__ImplementClass(Tools::IDLDocument, 'ILDC', Core::RefCounted);

using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
IDLDocument::IDLDocument()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
IDLDocument::Parse(XmlReader* reader)
{
    n_assert(0 != reader);
    n_assert(reader->GetCurrentNodeName() == "Nebula3");

    // parse Library elements
    if (reader->SetToFirstChild("Library")) do
    {
        Ptr<IDLLibrary> lib = IDLLibrary::Create();
        if (!lib->Parse(reader))
        {
            this->SetError(lib->GetError());
            return false;
        }
        this->libraries.Append(lib);
    }
    while (reader->SetToNextChild("Library"));

    // parse protocol elements
    if (reader->SetToFirstChild("Protocol")) do
    {
        Ptr<IDLProtocol> protocol = IDLProtocol::Create();
        if (!protocol->Parse(reader))
        {
            this->SetError(protocol->GetError());
            return false;
        }
        this->protocols.Append(protocol);
    }
    while (reader->SetToNextChild("Protocol"));

    return true;
}

} // namespace Tools