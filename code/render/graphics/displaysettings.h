#pragma once
//------------------------------------------------------------------------------
/**
    @class Graphics::DisplaySettings
    
    Wraps display settings into a simple object.
    
    (C) 2009 Radon Labs GmbH
*/
#include "core/types.h"
#include "coregraphics/adapter.h"
#include "coregraphics/displaymode.h"
#include "coregraphics/antialiasquality.h"
#include "coregraphics/pixelformat.h"
#include "coregraphics/adapterinfo.h"

//------------------------------------------------------------------------------
namespace Graphics
{
class DisplaySettings
{
public:
    /// constructor
    DisplaySettings();

    /// DisplayMode accessor
    CoreGraphics::DisplayMode& DisplayMode();
    /// const DisplayMode accessor
    const CoreGraphics::DisplayMode& DisplayMode() const;
    /// set display adapter to use
    void SetAdapter(CoreGraphics::Adapter::Code a);
    /// get displat adapter
    CoreGraphics::Adapter::Code GetAdapter() const;
    /// set antialias quality
    void SetAntiAliasQuality(CoreGraphics::AntiAliasQuality::Code aa);
    /// get antialias quality
    CoreGraphics::AntiAliasQuality::Code GetAntiAliasQuality() const;
    /// set windowed/fullscreen mode
    void SetFullscreen(bool b);
    /// get windowed/fullscreen mode
    bool IsFullscreen() const;
	/// set whether the size can be auto-adjusted to window's size
	void SetAutoAdjustSize(bool b);
	/// get whether the size can be auto-adjusted to window's size
	bool IsAutoAdjustSize() const;
    /// enable display mode switch when running fullscreen (default is true);
    void SetDisplayModeSwitchEnabled(bool b);
    /// is display mode switch enabled for fullscreen?
    bool IsDisplayModeSwitchEnabled() const;
    /// enable triple buffer for fullscreen (default is double buffering)
    void SetTripleBufferingEnabled(bool b);
    /// is triple buffer enabled for fullscreen?
    bool IsTripleBufferingEnabled() const;
    /// set always-on-top behaviour
    void SetAlwaysOnTop(bool b);
    /// get always-on-top behaviour
    bool IsAlwaysOnTop() const;
    /// turn vertical sync on/off
    void SetVerticalSyncEnabled(bool b);
    /// get vertical sync flag
    bool IsVerticalSyncEnabled() const;
    /// set optional window icon resource name
    void SetIconName(const Util::String& s);
    /// get optional window icon resource name
    const Util::String& GetIconName() const;
    /// set optional window title
    void SetWindowTitle(const Util::String& s);
    /// get optional window title
    const Util::String& GetWindowTitle() const;
    
private:
    CoreGraphics::Adapter::Code adapter;
    CoreGraphics::DisplayMode displayMode;
    CoreGraphics::AntiAliasQuality::Code antiAliasQuality;
    bool fullscreen;
	bool autoAdjustSize;
    bool modeSwitchEnabled;
    bool tripleBufferingEnabled;
    bool alwaysOnTop;
    bool verticalSync;
    Util::String iconName;
    Util::String windowTitle;
};

//------------------------------------------------------------------------------
/**
*/
inline CoreGraphics::DisplayMode&
DisplaySettings::DisplayMode()
{
    return this->displayMode;
}

//------------------------------------------------------------------------------
/**
*/
inline const CoreGraphics::DisplayMode&
DisplaySettings::DisplayMode() const
{
    return this->displayMode;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DisplaySettings::SetAntiAliasQuality(CoreGraphics::AntiAliasQuality::Code aa)
{
    this->antiAliasQuality = aa;
}

//------------------------------------------------------------------------------
/**
*/
inline CoreGraphics::AntiAliasQuality::Code
DisplaySettings::GetAntiAliasQuality() const
{
    return this->antiAliasQuality;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DisplaySettings::SetAdapter(CoreGraphics::Adapter::Code a)
{
    this->adapter = a;
}

//------------------------------------------------------------------------------
/**
*/
inline CoreGraphics::Adapter::Code
DisplaySettings::GetAdapter() const
{
    return this->adapter;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DisplaySettings::SetFullscreen(bool b)
{
    this->fullscreen = b;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DisplaySettings::SetAutoAdjustSize(bool b)
{
	this->autoAdjustSize = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
DisplaySettings::IsAutoAdjustSize() const
{
	return this->autoAdjustSize;
}

//------------------------------------------------------------------------------
inline bool
DisplaySettings::IsFullscreen() const
{
    return this->fullscreen;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DisplaySettings::SetDisplayModeSwitchEnabled(bool b)
{
    this->modeSwitchEnabled = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
DisplaySettings::IsDisplayModeSwitchEnabled() const
{
    return this->modeSwitchEnabled;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DisplaySettings::SetTripleBufferingEnabled(bool b)
{
    this->tripleBufferingEnabled = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
DisplaySettings::IsTripleBufferingEnabled() const
{
    return this->tripleBufferingEnabled;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DisplaySettings::SetAlwaysOnTop(bool b)
{
    this->alwaysOnTop = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
DisplaySettings::IsAlwaysOnTop() const
{
    return this->alwaysOnTop;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DisplaySettings::SetVerticalSyncEnabled(bool b)
{
    this->verticalSync = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
DisplaySettings::IsVerticalSyncEnabled() const
{
    return this->verticalSync;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DisplaySettings::SetIconName(const Util::String& s)
{
    this->iconName = s;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String&
DisplaySettings::GetIconName() const
{
    return this->iconName;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DisplaySettings::SetWindowTitle(const Util::String& t)
{
    this->windowTitle = t;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String&
DisplaySettings::GetWindowTitle() const
{
    return this->windowTitle;
}

} // namespace Graphics
//------------------------------------------------------------------------------

    