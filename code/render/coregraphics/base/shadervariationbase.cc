//------------------------------------------------------------------------------
//  shadervariationbase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/base/shadervariationbase.h"

namespace Base
{
__ImplementClass(Base::ShaderVariationBase, 'SVBS', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
ShaderVariationBase::ShaderVariationBase() :
    featureMask(0),
    numPasses(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ShaderVariationBase::~ShaderVariationBase()
{
    // empty
}

} // namespace Base
