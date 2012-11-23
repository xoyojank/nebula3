#pragma once
//------------------------------------------------------------------------------
/**
    @class UI::ProgressBarWindow

    @TODO implement me, moved from mangalore/ui/progressbarwindow.h

    (C) 2010 Radon Labs GmbH
*/
#include "ui/elements/window.h"

//------------------------------------------------------------------------------
namespace UI
{
class ProgressBarWindow : public Window
{
    __DeclareClass(ProgressBarWindow);
public:
    /// constructor
    ProgressBarWindow();
    /// destructor
    virtual ~ProgressBarWindow();

    /// enable/disable the debug text output
    void SetDebugTextEnabled(bool b);
    /// get debug text output enable flag
    bool GetDebugTextEnabled() const;

    /// open the window (will attach the window to the UI server)
    virtual void Open();

    /// set the progress max value
    void SetMaxProgressValue(int v);
    /// get the progress max value
    int GetMaxProgressValue() const;
	/// set the current progress value
	void SetCurProgressValue(int v);
	/// get the current progress value
	int GetCurProgressValue() const;
	/// advance the progress by some value
    void AdvanceProgress(int amount);
    
    /// set an optional text message
    void SetText(const Util::String& t);
    /// get text message
    const Util::String& GetText() const;

    /// do a full render loop (necessary for load/save screens)
    virtual void Present();
    /// wait for the ok button to be pressed
    virtual void WaitForOkButton();
    /// called to handle a gui event
    virtual void HandleEvent(Event* e);

protected:
/*
    /// show the optional Ok button
    void ShowOkButton();
    /// hide the optional Ok button
    void HideOkButton();
    /// set the visibility of an ui element
    void SetElementVisibility(const nString& elementName, bool visible);

    int rangeMaxValue;
    int rangeCurValue;
    nString text;
    bool debugTextEnabled;
    bool inputEnabled;
    bool okEventReceived;
*/
};

//------------------------------------------------------------------------------
/**
*/
inline
void
ProgressBarWindow::SetDebugTextEnabled(bool b)
{
//    this->debugTextEnabled = b;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
ProgressBarWindow::GetDebugTextEnabled() const
{
//    return this->debugTextEnabled;
    return false;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
ProgressBarWindow::SetMaxProgressValue(int v)
{
    //n_assert(v > 0);
    //this->rangeMaxValue = v;
}

//------------------------------------------------------------------------------
/**
*/
inline
int
ProgressBarWindow::GetMaxProgressValue() const
{
    //return this->rangeMaxValue;
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
ProgressBarWindow::SetCurProgressValue(int v)
{
    //n_assert(v >= 0);
	//this->rangeCurValue = n_min(v, this->rangeMaxValue);
}

//------------------------------------------------------------------------------
/**
*/
inline
int
ProgressBarWindow::GetCurProgressValue() const
{
    //return this->rangeCurValue;
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
ProgressBarWindow::AdvanceProgress(int amount)
{
    //this->rangeCurValue += amount;
    //this->rangeCurValue = n_min(this->rangeCurValue, this->rangeMaxValue);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
ProgressBarWindow::SetText(const Util::String& t)
{
    //this->text = t;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
ProgressBarWindow::GetText() const
{    
    //return this->text;
    static Util::String hack;
    return hack;
}

}; // namespace UI
//------------------------------------------------------------------------------
