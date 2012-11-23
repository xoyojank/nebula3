#pragma once 
#include "core/refcounted.h"
#include "util/string.h"
#include "util/array.h"

namespace Scaleforms
{
class Window;


class GfxParser : public  Core::RefCounted
{
	__DeclareClass(GfxParser);
public:
	/// constructor
	GfxParser();
	/// destructor
	virtual ~GfxParser();

	/// sets the filename of the ui.xml file
	void SetFileName(const Util::String& name);
	/// gets the filename
	const Util::String& GetFileName() const;

	/// opens the file and creates the gui structure
	Util::Array<Ptr<Window> > ParseFile();

private:
	Util::String fileName;
};
}