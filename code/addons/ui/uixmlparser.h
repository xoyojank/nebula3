#pragma once
//------------------------------------------------------------------------------
/**
    @class IO::UIExcelXmlReader
    
    This reader parses the XML - ui file and creates the UI tree.
	    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "util/string.h"
#include "util/array.h"
#include "ui/elements/window.h"
#include "ui/elements/button.h"
#include "ui/elements/label.h"
#include "ui/elements/frame.h"

namespace IO
{
	class XmlReader;
};

//------------------------------------------------------------------------------
namespace UI
{
class UIFactory;
class Window;

class UIXmlParser : public Core::RefCounted
{
    __DeclareClass(UIXmlParser);
public:
    /// constructor
    UIXmlParser();
    /// destructor
    virtual ~UIXmlParser();

	/// sets the filename of the ui.xml file
	void SetFileName(const Util::String& name);
	/// gets the filename
	const Util::String& GetFileName() const;

	/// opens the file and creates the gui structure
	Util::Array<Ptr<Window> > ParseFile();
    /// @GUI_LEGACY_DSA3
    void ParseFile(const Ptr<Window>& window);

private:
	/// parse the window node
	void ParseWindowNode(const Ptr<IO::XmlReader>& xml, const Ptr<Window>& window);
	/// parse the button node
	void ParseCanvasNode(const Ptr<IO::XmlReader>& xml, const Ptr<Window>& parent);

	/// parses a single element
	void ParseChildElements(const Ptr<IO::XmlReader>& xml, const Ptr<Element>& parent);

	/// parse the button node
	void ParseButtonNode(const Ptr<IO::XmlReader>& xml, const Ptr<Element>& parent);
	/// parse the label node
	void ParseLabelNode(const Ptr<IO::XmlReader>& xml, const Ptr<Element>& parent);
    /// parse a frame node
    void ParseFrameNode(const Ptr<IO::XmlReader>& xml, const Ptr<Element>& parent);

    /// recursively link neighbours of element
    void LinkElementNeighbors(const Ptr<Element>& element);

	/// helper functions
	bool                    ElementGetVisible(const Ptr<IO::XmlReader>& xml) const;
	Math::rectangle<float>  ElementGetScreenSpaceRect(const Ptr<IO::XmlReader>& xml) const;
	Math::float2    ElementGetSize(const Ptr<IO::XmlReader>& xml) const;
	Util::String    ElementGetId(const Ptr<IO::XmlReader>& xml) const;	
	Math::float4    ElementGetUV(const Ptr<IO::XmlReader>& xml) const;
	Math::float4    ElementGetVisualStateUV(const Ptr<IO::XmlReader>& xml, Button::VisualState state) const;
    Math::float4    ElementGetBlinkColor(const Ptr<IO::XmlReader>& xml) const;
    float           ElementGetBlinkTime(const Ptr<IO::XmlReader>& xml) const;
    float           ElementGetAnimFadeTime(const Ptr<IO::XmlReader>& xml) const;
    SizeT           ElementGetAnimElementCountX(const Ptr<IO::XmlReader>& xml) const;
    SizeT           ElementGetAnimElementCountY(const Ptr<IO::XmlReader>& xml) const;

    /// optional
    Math::float4    ElementGetColor(const Ptr<IO::XmlReader>& xml) const;
    bool            ElementIgnoreInput(const Ptr<IO::XmlReader>& xml) const;
    bool            ElementGetEnabled(const Ptr<IO::XmlReader>& xml) const;
    bool            ElementGetBlinking(const Ptr<IO::XmlReader>& xml) const;

	Util::String fileName;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
UIXmlParser::SetFileName(const Util::String& name)
{
	this->fileName = name;
}


//------------------------------------------------------------------------------
/**
*/
inline
const Util::String& 
UIXmlParser::GetFileName() const
{
	return this->fileName;
}

} // namespace UI
//------------------------------------------------------------------------------
