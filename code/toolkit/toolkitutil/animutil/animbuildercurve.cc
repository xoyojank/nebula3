//------------------------------------------------------------------------------
//  animbuildercurve.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "toolkitutil/animutil/animbuildercurve.h"

namespace ToolkitUtil
{

using namespace CoreAnimation;

//------------------------------------------------------------------------------
/**
*/
AnimBuilderCurve::AnimBuilderCurve() :
    staticKey(0.0f, 0.0, 0.0f, 0.0f),
    firstKeyIndex(InvalidIndex),
    curveType(CurveType::InvalidCurveType),
    isActive(false),
    isStatic(false)
{
    // empty
}
    
//------------------------------------------------------------------------------
/**
*/
void
AnimBuilderCurve::ResizeKeyArray(SizeT numKeys)
{
    this->keyArray.Resize(numKeys);
}

} // namespace ToolkitUtil