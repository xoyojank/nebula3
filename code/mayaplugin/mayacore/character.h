#pragma once
//------------------------------------------------------------------------------
/**
    @class Characters::Character

    A Character resource object holds all shared data belonging to a 
    skinned character. 

    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "mayacore/characterskeleton.h"
#include "mayacore/characterskinlist.h"

//------------------------------------------------------------------------------
namespace Characters
{

class Character : public Core::RefCounted
{
    __DeclareClass(Character);
public:
    /// constructor
    Character();
    /// destructor
    virtual ~Character();

    /// access to the character's skeleton
    CharacterSkeleton& Skeleton();
	// write data to stream
	virtual bool WriteDataTag(Ptr<ToolkitUtil::ModelWriter>& writer);

private:
    CharacterSkeleton skeleton;
	Util::Array<CharacterSkinList> skinLists;
    bool isValid;
};

//------------------------------------------------------------------------------
/**
*/
inline CharacterSkeleton&
Character::Skeleton()
{
    return this->skeleton;
}

} // namespace Characters
//------------------------------------------------------------------------------



    