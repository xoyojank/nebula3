//------------------------------------------------------------------------------
//  attrexithandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "attr/attrexithandler.h"
#include "attr/attributedefinitionbase.h"

namespace Attr
{

//------------------------------------------------------------------------------
/**
*/
void
AttrExitHandler::OnExit() const
{
    // cleanup dynamic attributes
    AttributeDefinitionBase::ClearDynamicAttributes();
}

} // namespace Attr