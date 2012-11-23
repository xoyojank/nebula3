//------------------------------------------------------------------------------
//  characterskinlist.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "characters/characterskinlist.h"

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

} // namespace Characters
