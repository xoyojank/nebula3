#ifndef _Nebula3Exporter
#define _Nebula3Exporter

#include <maya/MPxFileTranslator.h>

//------------------------------------------------------------------------------
namespace Maya
{
class Nebula3Exporter : public MPxFileTranslator
{
public:
	Nebula3Exporter();
	virtual			~Nebula3Exporter(); 
	static void*    creator();

	MStatus         reader (const MFileObject& file,const MString& optionsString,FileAccessMode mode);
	MStatus         writer (const MFileObject& file,const MString& optionsString,FileAccessMode mode );
	bool            haveReadMethod () const;
	bool            haveWriteMethod () const;
	MString         defaultExtension () const;
	MFileKind       identifyFile ( const MFileObject& fileName,const char* buffer,short size) const;
};

//------------------------------------------------------------------------------
/**
*/
inline
bool Nebula3Exporter::haveReadMethod () const
{
	return false;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool Nebula3Exporter::haveWriteMethod () const
{
	return true;
}

//------------------------------------------------------------------------------
/**
*/
inline
MString Nebula3Exporter::defaultExtension () const
{
	return "n3";
}

inline
MPxFileTranslator::MFileKind Nebula3Exporter::identifyFile ( const MFileObject& fileName,const char* buffer,short size) const
{
	const char * name = fileName.name().asChar();
	int   nameLength = strlen(name);

	if ((nameLength > 3) && !strcasecmp(name+nameLength-3, ".n3"))
		return kCouldBeMyFileType;
	else
		return kNotMyFileType;
}

} // namespace Maya
//------------------------------------------------------------------------------


#endif
