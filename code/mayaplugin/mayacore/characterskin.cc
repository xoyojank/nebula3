//------------------------------------------------------------------------------
//  characterskin.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "characters/characterskin.h"

namespace Characters
{
using namespace Models;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
CharacterSkin::CharacterSkin()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
CharacterSkin::CharacterSkin(const Ptr<ModelNode>& skinModelNode, const StringAtom& skinCategory, const StringAtom& skinName) :
    modelNode(skinModelNode),
    category(skinCategory),
    name(skinName)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
CharacterSkin::~CharacterSkin()
{
    // empty
}

} // namespace Characters