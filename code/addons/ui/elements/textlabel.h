#pragma once
//------------------------------------------------------------------------------
/**
    @class UI::TextLabel

    A label that can display text. Currently a placeholder.
    
    (C) 2005 Radon Labs GmbH
*/
#include "ui/elements/label.h"

//------------------------------------------------------------------------------
namespace UI
{
class TextLabel : public Label
{
	__DeclareClass(TextLabel);
public:
    /// constructor
    TextLabel();
    /// destructor
    virtual ~TextLabel();

    /// set label text
    void SetText(const Util::String& text);
    /// get label text
    const Util::String& GetText() const;

private:
    Util::String text;
};

//------------------------------------------------------------------------------
/**
*/
inline void
TextLabel::SetText(const Util::String& t)
{
    this->text = t;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String&
TextLabel::GetText() const
{
    return this->text;
}

} // namespace UI
//------------------------------------------------------------------------------