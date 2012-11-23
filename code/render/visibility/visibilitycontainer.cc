//------------------------------------------------------------------------------
//  entityvisibility.cc
//  (C) 2010 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "visibility/visibilitycontainer.h"

namespace Visibility
{
__ImplementClass(Visibility::VisibilityContainer, 'VICT', Core::RefCounted);

using namespace Util;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
VisibilityContainer::VisibilityContainer()
{
}

//------------------------------------------------------------------------------
/**
*/
VisibilityContainer::~VisibilityContainer()
{
}

} // namespace Visibility
