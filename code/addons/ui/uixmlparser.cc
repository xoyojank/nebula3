//------------------------------------------------------------------------------
//  uixmlparser.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "ui/uixmlparser.h"
#include "ui/uifactory.h"
#include "ui/elements/canvas.h"
#include "io/xmlreader.h"
#include "io/filestream.h"
#include "io/ioserver.h"
#include "ui/uimanager.h"

namespace UI
{
__ImplementClass(UIXmlParser, 'UIPA', Core::RefCounted);

using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
UIXmlParser::UIXmlParser() : fileName("")
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
UIXmlParser::~UIXmlParser()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void 
UIXmlParser::ParseFile(const Ptr<Window>& window)
{
	// open xml 
    URI uri = "export:ui/" + this->fileName;
	Ptr<XmlReader> xml = XmlReader::Create();
    xml->SetStream(IoServer::Instance()->CreateStream(uri));
	if (!xml->Open())
    {
        n_error("UIXMLParser: Could not open '%s' as UI XML file!\n", uri.LocalPath().AsCharPtr());
    }
    
	// check root node
	String nodeName = xml->GetCurrentNodeName();
	n_assert("Nebula3" == nodeName);

	// get next node.. has to be a window node
	if(!xml->SetToFirstChild("Window"))
	{
        n_assert(false);
    }
    this->ParseWindowNode(xml, window);
}

//------------------------------------------------------------------------------
/**
*/
Array<Ptr<Window> >
UIXmlParser::ParseFile()
{
	// open xml 
	Ptr<XmlReader> xml = XmlReader::Create();
    xml->SetStream(IoServer::Instance()->CreateStream("export:ui/" + this->fileName));
	xml->Open();
	n_assert(xml->IsOpen());

	// check root node
	String nodeName = xml->GetCurrentNodeName();
	n_assert("Nebula3" == nodeName);

	// get next node.. has to be a window node
    Array<Ptr<Window> > result;
	if(xml->SetToFirstChild("Window"))
	{
		do
		{
            Ptr<Window> newWindow = Window::Create();
			this->ParseWindowNode(xml, newWindow);
            result.Append(newWindow);
		}
        while(xml->SetToNextChild());
	}
    return result;
}

//------------------------------------------------------------------------------
/**
*/
void
UIXmlParser::ParseWindowNode(const Ptr<XmlReader>& xml, const Ptr<Window>& window)
{
	n_assert(xml->GetCurrentNodeName() == "Window");

    // @GUI_LEGACY_DSA3
    window->SetDsaLegacyMode(UIManager::Instance()->GetDsaLegacyModeFlag());

	// parse node	
	String xalign = xml->GetString("xAlign");
	String yalign = xml->GetString("yAlign");
	window->SetScreenAlignmentX(Window::StringToAlignmentX(xalign));
	window->SetScreenAlignmentY(Window::StringToAlignmentY(yalign));
	window->SetWindowTitle(xml->GetString("title"));
    window->SetWindowTexture(xml->GetString("texture"));
	
	// now parse the elements in the window node
	// first has to be a canvas
	xml->SetToFirstChild();
    this->ParseCanvasNode(xml, window);
	window->UpdateAlignedPosition();
	xml->SetToParent();
}

//------------------------------------------------------------------------------
/**
*/
void
UIXmlParser::ParseCanvasNode(const Ptr<XmlReader>& xml, const Ptr<Window>& parent)
{
	n_assert(xml->GetCurrentNodeName() == "Canvas");

	// parse node
    Ptr<Canvas> newCanvas = UIFactory::Instance()->CreateElement("Canvas").cast<Canvas>();
	newCanvas->SetId(this->ElementGetId(xml));
	newCanvas->SetColor(this->ElementGetColor(xml));	
	newCanvas->SetUV(this->ElementGetUV(xml));
	newCanvas->SetVisible(this->ElementGetVisible(xml));
	newCanvas->SetEnabled(this->ElementGetEnabled(xml));
	newCanvas->SetScreenSpaceRect(this->ElementGetScreenSpaceRect(xml));	
	newCanvas->SetParentWindow(parent);
	
	// set window canvas
	parent->SetCanvas(newCanvas);
	
	// call directly OnCreate() cause this is called in AttachElement, but
	// a canvas will never be attached like other elements
	newCanvas->OnCreate();

	Ptr<Element> el = newCanvas.cast<Element>();

	// parse childs recursivly, if there is one
	if(xml->SetToFirstChild())
	{		
		do
		{
			this->ParseChildElements(xml, el);
		}
        while(xml->SetToNextChild());
	}

    // link neighbors of canvas and its children
    this->LinkElementNeighbors(el);
}

//------------------------------------------------------------------------------
/**
*/
void
UIXmlParser::ParseChildElements(const Ptr<XmlReader>& xml, const Ptr<Element>& parent)
{
    String nodeType = xml->GetCurrentNodeName();
    if ("Frame" == nodeType)        this->ParseFrameNode(xml, parent);
    else if ("Button" == nodeType)  this->ParseButtonNode(xml, parent);
    else if	("Label" == nodeType)   this->ParseLabelNode(xml, parent);
}

//------------------------------------------------------------------------------
/**
*/
void
UIXmlParser::ParseButtonNode(const Ptr<XmlReader>& xml, const Ptr<Element>& parent)
{
	n_assert(xml->GetCurrentNodeName() == "Button");

	// parse node
    Ptr<Button> newButton = UIFactory::Instance()->CreateElement("Button").cast<Button>();
	newButton->SetId(this->ElementGetId(xml));
	newButton->SetColor(this->ElementGetColor(xml));	
	newButton->SetUV(this->ElementGetUV(xml));
	newButton->SetVisible(this->ElementGetVisible(xml));
	newButton->SetEnabled(this->ElementGetEnabled(xml));
	newButton->SetScreenSpaceRect(this->ElementGetScreenSpaceRect(xml));	
    if (xml->HasAttr("event"))
    {
        newButton->SetEventName(xml->GetString("event"));
    }
    if (xml->HasAttr("texture"))
    {
        newButton->SetStateTexture(Button::Normal, xml->GetString("texture"));
    }
    if (xml->HasAttr("pressedTexture"))
    {
        newButton->SetStateTexture(Button::Pressed, xml->GetString("pressedTexture"));
    }
    if (xml->HasAttr("mouseOverTexture"))
    {
        newButton->SetStateTexture(Button::MouseOver, xml->GetString("mouseOverTexture"));
    }
    if (xml->HasAttr("disabledTexture"))
    {
        newButton->SetStateTexture(Button::Disabled, xml->GetString("disabledTexture"));
    }

	IndexT stateIndex;
	for(stateIndex = 0; stateIndex < Button::NumVisualStates; stateIndex++)
	{
		newButton->SetStateUV((Button::VisualState)stateIndex, this->ElementGetVisualStateUV(xml, (Button::VisualState)stateIndex));
	}

	// and append
	Ptr<Element> el = newButton.cast<Element>();
	parent->AttachElement(el);
	
	// check for further childs
	if(xml->SetToFirstChild())
	{
		do
		{
			this->ParseChildElements(xml, el);
		}
        while(xml->SetToNextChild());
	}
}

//------------------------------------------------------------------------------
/**
*/
void
UIXmlParser::ParseLabelNode(const Ptr<XmlReader>& xml, const Ptr<Element>& parent)
{
	n_assert(xml->GetCurrentNodeName() == "Label");

	// parse node
    Ptr<Label> newLabel = UIFactory::Instance()->CreateElement("Label").cast<Label>();
	newLabel->SetId(this->ElementGetId(xml));
	newLabel->SetColor(this->ElementGetColor(xml));	
	newLabel->SetUV(this->ElementGetUV(xml));
	newLabel->SetVisible(this->ElementGetVisible(xml));
	newLabel->SetEnabled(this->ElementGetEnabled(xml));
	newLabel->SetScreenSpaceRect(this->ElementGetScreenSpaceRect(xml));	
    if (xml->HasAttr("texture"))
    {
        newLabel->SetTexture(xml->GetString("texture"));
    }

	// and append
	Ptr<Element> el = newLabel.cast<Element>();
	parent->AttachElement(el);

	// check for further childs
	if (xml->SetToFirstChild())
	{
		do
		{
			this->ParseChildElements(xml, el);
		}
        while(xml->SetToNextChild());
	}
}

//------------------------------------------------------------------------------
/**
*/
void
UIXmlParser::ParseFrameNode(const Ptr<XmlReader>& xml, const Ptr<Element>& parent)
{
	n_assert(xml->GetCurrentNodeName() == "Frame");

    Ptr<Frame> newFrame = UIFactory::Instance()->CreateElement("Frame").downcast<Frame>();
	newFrame->SetId(this->ElementGetId(xml));
	newFrame->SetColor(this->ElementGetColor(xml));	
	newFrame->SetUV(this->ElementGetUV(xml));
	newFrame->SetVisible(this->ElementGetVisible(xml));
	newFrame->SetEnabled(this->ElementGetEnabled(xml));
	newFrame->SetScreenSpaceRect(this->ElementGetScreenSpaceRect(xml));	
    if (xml->HasAttr("texture"))
    {
        newFrame->SetTexture(xml->GetString("texture"));
    }
    const Ptr<Element>& elm = newFrame.upcast<Element>();
	parent->AttachElement(elm);

	// check for further childs
	if (xml->SetToFirstChild())
	{
		do
		{
			this->ParseChildElements(xml, elm);
		}
        while(xml->SetToNextChild());
	}

}

//------------------------------------------------------------------------------
/**
*/
bool
UIXmlParser::ElementGetVisible(const Ptr<XmlReader>& xml) const
{
	// optional
	if(xml->HasAttr("visible"))		return xml->GetBool("visible");
	return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
UIXmlParser::ElementGetEnabled(const Ptr<XmlReader>& xml) const
{
	// optional
	if(xml->HasAttr("enabled"))		return xml->GetBool("enabled");
	return true;
}
//------------------------------------------------------------------------------
/**
*/
Math::rectangle<float>
UIXmlParser::ElementGetScreenSpaceRect(const Ptr<XmlReader>& xml) const
{
    Math::rectangle<float> rect;
    Math::float4 val = xml->GetFloat4("rect");
    rect.left   = val.x();
    rect.right  = val.z();
    rect.top    = val.y();
    rect.bottom = val.w();
    return rect;
}

//------------------------------------------------------------------------------
/**
*/
String 
UIXmlParser::ElementGetId(const Ptr<XmlReader>& xml) const
{
	return xml->GetString("id");
}

//------------------------------------------------------------------------------
/**
*/
Math::float4 
UIXmlParser::ElementGetColor(const Ptr<XmlReader>& xml) const
{
	if(xml->HasAttr("defaultColor"))		return xml->GetFloat4("defaultColor");
	return Math::float4(1,1,1,1);
}

//------------------------------------------------------------------------------
/**
*/
Math::float4 
UIXmlParser::ElementGetUV(const Ptr<XmlReader>& xml) const
{	
	return xml->GetFloat4("defaultUV");
}

//------------------------------------------------------------------------------
/**
*/
Math::float4 
UIXmlParser::ElementGetVisualStateUV(const Ptr<XmlReader>& xml, Button::VisualState state) const
{	
	switch(state)
	{
	case Button::Normal:
		return xml->GetFloat4("defaultUV");
	case Button::Pressed:
        if (xml->HasAttr("pressedUV")) return xml->GetFloat4("pressedUV");
        else                           return xml->GetFloat4("defaultUV");
	case Button::MouseOver:
		if (xml->HasAttr("mouseOverUV")) return xml->GetFloat4("mouseOverUV");
        else                             return xml->GetFloat4("defaultUV");
	case Button::Disabled:
        if (xml->HasAttr("disabledUV"))  return xml->GetFloat4("disabledUV");
        else                             return xml->GetFloat4("defaultUV");
	default:
		n_error("Wrong visual state requested in UIXmlParser::ElementGetUV");
	}
	// dummy return, to prevent compiler waring
	return Math::float4(0,0,0,0);
}

//------------------------------------------------------------------------------
/**
*/
SizeT
UIXmlParser::ElementGetAnimElementCountX(const Ptr<XmlReader>& xml) const
{
    return xml->GetInt("animElementCountX");
}

//------------------------------------------------------------------------------
/**
*/
SizeT
UIXmlParser::ElementGetAnimElementCountY(const Ptr<XmlReader>& xml) const
{
    return xml->GetInt("animElementCountY");
}

//------------------------------------------------------------------------------
/**
    Recursively link all neighbor elements of all children elements
*/
void 
UIXmlParser::LinkElementNeighbors(const Ptr<Element>& element)
{
    element->LinkNeighbours();

    // go thru children
    const Array<Ptr<Element> >& children = element->GetChildElements();
    IndexT i;
    for (i = 0; i < children.Size(); ++i)
    {
    	this->LinkElementNeighbors(children[i]);
    }
}
} // namespace UI
