#include "stdneb.h"
#include "gfxparser.h"


namespace Scaleforms
{
__ImplementClass(GfxParser, 'GFPR', Core::RefCounted);

using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
GfxParser::GfxParser() : fileName("")
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
GfxParser::~GfxParser()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
Array<Ptr<Window> >
GfxParser::ParseFile()
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
}
