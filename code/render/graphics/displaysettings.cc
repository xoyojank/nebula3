//------------------------------------------------------------------------------
//  displaysettings.cc
//  (C) 2010 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/displaysettings.h"

namespace Graphics
{

using namespace Util;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
DisplaySettings::DisplaySettings() :
    adapter(Adapter::Primary),
    displayMode(0, 0, 1024, 768, PixelFormat::X8R8G8B8),
    antiAliasQuality(AntiAliasQuality::None),
    fullscreen(false),
	autoAdjustSize(true),
    modeSwitchEnabled(true),
    tripleBufferingEnabled(false),
    alwaysOnTop(false),
    verticalSync(false),
    iconName("NebulaIcon"),
    windowTitle("Nebula3 Application Window")
{
    // empty
}

} // namespace Graphics
