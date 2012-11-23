//------------------------------------------------------------------------------
//  character.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "mayacore/character.h"

namespace Characters
{
__ImplementClass(Characters::Character, 'CRTR', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
Character::Character() :
    isValid(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Character::~Character()
{
}

//------------------------------------------------------------------------------
/**
*/
bool 
Character::WriteDataTag(Ptr<ToolkitUtil::ModelWriter>& writer)
{
	this->skeleton.WriteDataTag(writer);
	for(IndexT index = 0 ; index < skinLists.Size();index++ )
	{
		this->skinLists[index].WriteDataTag(writer);
	}
	
	return true;
}

} // namespace Characters
