//------------------------------------------------------------------------------
//  script/dialog/dialogdesc.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "scriptfeature/dialog/dialogdesc.h"

namespace Script
{
__ImplementClass(Script::DialogDesc, 'DDES', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
DialogDesc::DialogDesc() :
    isLocked(false),
    isConversation(false)
{
    // empty
}

} // namespace Script