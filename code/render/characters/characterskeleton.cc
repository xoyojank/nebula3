//------------------------------------------------------------------------------
//  characterskeleton.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "characters/characterskeleton.h"

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
    this->defaultSamplesArray.SetSize(numJoints * 4);
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

    // also update the default samples array which will be used if no animation is
    // set on the character as input to the asynchronous skeleton evaluation
    // the sample array always has 4 "channels": translation, rotation, scale and velocity
    // NOTE: translation is actually used as a vector during skeleton evaluation
    this->defaultSamplesArray[jointIndex * 4 + 0] = vector(poseTranslation);
    this->defaultSamplesArray[jointIndex * 4 + 1].load((const float*)&poseRotation);
    this->defaultSamplesArray[jointIndex * 4 + 2] = poseScale;
    this->defaultSamplesArray[jointIndex * 4 + 3] = vector::nullvec();  // velocity
}

} // namespace Characters