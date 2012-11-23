//------------------------------------------------------------------------------
//  characterskeleton.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "mayacore/characterskeleton.h"

namespace Characters
{
using namespace Util;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
CharacterSkeleton::CharacterSkeleton() :
    isValid(false)    
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
CharacterSkeleton::~CharacterSkeleton()
{
    n_assert(!this->IsValid());
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterSkeleton::Setup(SizeT numJoints)
{
    n_assert(!this->IsValid());
    n_assert((numJoints > 0) && (numJoints < 256)); // 256-limitation because of job system!
    this->invPoseMatrixArray.SetSize(numJoints);
    this->jointArray.SetSize(numJoints);
    this->jointIndexMap.Reserve(numJoints);
    this->isValid = true;
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterSkeleton::Discard()
{
    n_assert(this->IsValid());
    this->isValid = false;
    this->jointArray.Clear();
    this->jointIndexMap.Clear();
    this->invPoseMatrixArray.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterSkeleton::SetupJoint(IndexT jointIndex, IndexT parentJointIndex, const point& poseTranslation, const quaternion& poseRotation, const vector& poseScale, const StringAtom& name)
{
    n_assert(this->IsValid());
    n_assert(!this->jointIndexMap.Contains(name));
    CharacterJoint& curJoint = this->jointArray[jointIndex];
    CharacterJoint* parentJoint = 0;
    if (InvalidIndex != parentJointIndex)
    {
        parentJoint = &(this->jointArray[parentJointIndex]);
    }
    curJoint.Setup(parentJointIndex, parentJoint, poseTranslation, poseRotation, poseScale, name, &(this->invPoseMatrixArray[jointIndex]));
    this->jointIndexMap.Add(name, jointIndex);
}

//------------------------------------------------------------------------------
/**
*/
bool 
CharacterSkeleton::WriteDataTag(Ptr<ToolkitUtil::ModelWriter>& writer)
{
	for(IndexT index = 0; index < this->jointArray.Size(); index++)
	{
		this->jointArray[index].WriteDataTag(writer);
	}
	return true;
}

} // namespace Characters