//------------------------------------------------------------------------------
//  streamtexturesaverbase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/base/streamtexturesaverbase.h"

namespace Base
{
__ImplementClass(Base::StreamTextureSaverBase, 'STSB', Resources::ResourceSaver);

using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
StreamTextureSaverBase::StreamTextureSaverBase() :
    format(ImageFileFormat::JPG),
    mipLevel(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
StreamTextureSaverBase::~StreamTextureSaverBase()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
StreamTextureSaverBase::OnSave()
{
    // empty, override in subclass!
    return false;
}

} // namespace Base
