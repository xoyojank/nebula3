#pragma once
//------------------------------------------------------------------------------
/**
    @class Characters::CharacterJoint
    
    Holds shared data of a character joint.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/types.h"
#include "util/stringatom.h"
#include "math/point.h"
#include "math/vector.h"
#include "math/quaternion.h"
#include "math/matrix44.h"
//#include "models/modelwriter.h"

//------------------------------------------------------------------------------
namespace Characters
{
class CharacterJoint
{
public:
    /// constructor
    CharacterJoint();
    /// destructor
    ~CharacterJoint();
    /// setup the joint
    void Setup(IndexT parentJointIndex, const CharacterJoint* parentJoint, const Math::vector& poseTranslation, const Math::quaternion& poseRotation, const Math::vector& poseScale, const Util::StringAtom& name, Math::matrix44* invPoseMatrixPtr);

#if NEBULA3_EDITOR
	/// get the joint's name
	void SetName(const Util::StringAtom& jointName);
	/// set joint index
	void SetJointIndex(IndexT index);
	/// get the parent joint index
	IndexT GetJointIndex( ) const;
	/// get the parent joint index
	void SetParentJointIndex(IndexT index);
	/// get pose translation
	void SetPoseTranslation(const Math::vector& poseTranslation);
	/// get pose rotation
	void SetPoseRotation(const Math::quaternion& poseRotation);
	/// get pose scale
	void SetPoseScale( const Math::vector& poseScale);
	/// get the pose matrix
	void SetPoseMatrix( const Math::matrix44& poseMatirx);
	/// get the inverse pose matrix
	void SetInvPoseMatrix(const Math::matrix44& InvposeMatirx);
	// write data to stream
	virtual bool WriteDataTag(Ptr<Models::ModelWriter>& writer);
#endif

    /// get the joint's name
    const Util::StringAtom& GetName() const;
    /// return true if the joint has a parent joint
    bool HasParentJoint() const;
    /// get the parent joint index
    IndexT GetParentJointIndex() const;
    /// get pointer to parent joint (can be 0!)
    const CharacterJoint* GetParentJoint() const;
    /// get pose translation
    const Math::vector& GetPoseTranslation() const;
    /// get pose rotation
    const Math::quaternion& GetPoseRotation() const;
    /// get pose scale
    const Math::vector& GetPoseScale() const;
    /// get the pose matrix
    const Math::matrix44& GetPoseMatrix() const;
    /// get the inverse pose matrix
    const Math::matrix44& GetInvPoseMatrix() const;

private:
    Math::vector poseTranslation;
    Math::quaternion poseRotation;
    Math::vector poseScale;
    Math::matrix44 poseMatrix;
    Math::matrix44* invPoseMatrixPtr;
    IndexT parentJointIndex;
    const CharacterJoint* parentJoint;
    Util::StringAtom name;
	IndexT JointIndex;
};

#if NEBULA3_EDITOR
//------------------------------------------------------------------------------
/**
*/
inline void
CharacterJoint::SetName(const Util::StringAtom& jointName)
{
	this->name = jointName;
}

//------------------------------------------------------------------------------
/**
*/
inline void
CharacterJoint::SetJointIndex(IndexT index)
{
	this->JointIndex = index;
}

//------------------------------------------------------------------------------
/**
*/
inline void
CharacterJoint::SetParentJointIndex(IndexT index)
{
	this->parentJointIndex = index;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT 
CharacterJoint::GetJointIndex( ) const
{
	return this->JointIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline void
CharacterJoint::SetPoseTranslation(const Math::vector& poseTranslation_)
{
	this->poseTranslation = poseTranslation_;
}

//------------------------------------------------------------------------------
/**
*/
inline void
CharacterJoint::SetPoseRotation(const Math::quaternion& poseRotation_)
{
	this->poseRotation = poseRotation_;
}

//------------------------------------------------------------------------------
/**
*/
inline void
CharacterJoint::SetPoseScale( const Math::vector& poseScale_)
{
	this->poseScale = poseScale_;
}

//------------------------------------------------------------------------------
/**
*/
inline void
CharacterJoint::SetPoseMatrix( const Math::matrix44& poseMatirx_)
{
	this->poseMatrix = poseMatirx_;
}

//------------------------------------------------------------------------------
/**
*/
inline void
CharacterJoint::SetInvPoseMatrix(const Math::matrix44& InvposeMatirx)
{
}
#endif

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
CharacterJoint::GetName() const
{
    return this->name;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
CharacterJoint::HasParentJoint() const
{
    return (InvalidIndex != this->parentJointIndex);
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
CharacterJoint::GetParentJointIndex() const
{
    return this->parentJointIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline const CharacterJoint*
CharacterJoint::GetParentJoint() const
{
    return this->parentJoint;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::vector&
CharacterJoint::GetPoseTranslation() const
{
    return this->poseTranslation;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::quaternion&
CharacterJoint::GetPoseRotation() const
{
    return this->poseRotation;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::vector&
CharacterJoint::GetPoseScale() const
{
    return this->poseScale;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
CharacterJoint::GetPoseMatrix() const
{
    return this->poseMatrix;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
CharacterJoint::GetInvPoseMatrix() const
{
    n_assert(0 != this->invPoseMatrixPtr);
    return *this->invPoseMatrixPtr;
}

} // namespace Characters
//------------------------------------------------------------------------------
    