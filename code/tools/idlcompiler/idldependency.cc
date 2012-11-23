//------------------------------------------------------------------------------
//  idldependency.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/idlcompiler/idldependency.h"

namespace Tools
{
__ImplementClass(Tools::IDLDependency, 'ILDP', Core::RefCounted);

using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
IDLDependency::IDLDependency()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
IDLDependency::Parse(XmlReader* reader)
{
    n_assert(0 != reader);
    n_assert(reader->GetCurrentNodeName() == "Dependency");
    this->header = reader->GetString("header");
    return true;
}

} // namespace Tools