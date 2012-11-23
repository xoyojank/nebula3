//------------------------------------------------------------------------------
//  characterskinlist.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "mayacore/characterskinlist.h"


namespace Characters
{
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
CharacterSkinList::CharacterSkinList()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
CharacterSkinList::~CharacterSkinList()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterSkinList::SetName(const StringAtom& n)
{
    this->name = n;
}

//------------------------------------------------------------------------------
/**
*/
const StringAtom&
CharacterSkinList::GetName() const
{
    return this->name;
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterSkinList::SetSkins(const Array<StringAtom>& s)
{
    this->skins = s;
}

//------------------------------------------------------------------------------
/**
*/
const Array<Util::StringAtom>&
CharacterSkinList::GetSkins() const
{
    return this->skins;
}

//------------------------------------------------------------------------------
/**
*/
bool 
CharacterSkinList::WriteDataTag(Ptr<ToolkitUtil::ModelWriter>& writer)
{
	// write skin list to n3 file
	writer->BeginTag("SkinList", 'SKNL');
	writer->WriteString(name.AsString());
	writer->WriteInt(skins.Size());
	for (IndexT skinIndex = 0; skinIndex < skins.Size(); skinIndex++)
	{
		writer->WriteString(skins[skinIndex].AsString());
	}
	writer->EndTag();
	return true;
}

} // namespace Characters
