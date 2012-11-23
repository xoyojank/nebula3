//------------------------------------------------------------------------------
//  characterjoint.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "characters/characterjoint.h"

namespace Characters
{
using namespace Util;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
CharacterJoint::CharacterJoint() :
    poseTranslation(vector::nullvec()),
    poseRotation(quaternion::identity()),
    poseScale(1.0f, 1.0f, 1.0f),
    poseMatrix(matrix44::identity()),
    invPoseMatrixPtr(0),
    parentJointIndex(InvalidIndex),
    parentJoint(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
CharacterJoint::~CharacterJoint()
{
    this->parentJointIndex = InvalidIndex;
    this->parentJoint = 0;
    this->name.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterJoint::Setup(IndexT parentIndex, const CharacterJoint* parent, const vector& t, const quaternion& r, const vector& s, const StringAtom& n, matrix44* invPoseMatrixPtr_)
{
    n_assert(0 != invPoseMatrixPtr_);

    this->poseTranslation = t;
    this->poseRotation = r;
    this->poseScale = s;
    this->parentJointIndex = parentIndex;
    this->parentJoint = parent;
    this->name = n;

    this->poseMatrix = matrix44::identity();
    this->poseMatrix.scale(this->poseScale);
    this->poseMatrix = matrix44::multiply(this->poseMatrix, matrix44::rotationquaternion(this->poseRotation));
    this->poseMatrix.translate(this->poseTranslation);
    if (0 != parent)
    {
        this->poseMatrix = matrix44::multiply(this->poseMatrix, parent->poseMatrix);
    }
    this->invPoseMatrixPtr = invPoseMatrixPtr_;
    *this->invPoseMatrixPtr = matrix44::inverse(this->poseMatrix);
}

#if NEBULA3_EDITOR
//------------------------------------------------------------------------------
/**
*/
bool
CharacterJoint::WriteDataTag(Ptr<Models::ModelWriter>& writer)
{
	writer->BeginTag("Joint", 'JONT');
	writer->WriteInt(this->JointIndex);
	writer->WriteInt(this->parentJointIndex);
	writer->WriteFloat4(this->poseTranslation);
	writer->WriteFloat4(Math::float4(this->poseRotation.x(),this->poseRotation.y(),this->poseRotation.z(),this->poseRotation.w()));
	writer->WriteFloat4(this->poseScale);
	writer->WriteString(this->name.AsString());
	writer->EndTag();
	return false;
}
#endif

} // namespace Characters
