#pragma once
//------------------------------------------------------------------------------
/**
    @class NebulaMotionState

    (C) 2006 Radon Labs GmbH
*/

#include "LinearMath/btMotionState.h"
#include "bulletnebulaconversion.h"

class NebulaMotionState : public btMotionState
{
public:	
	NebulaMotionState() {}

	virtual void getWorldTransform(btTransform& worldTrans) const
	{
        worldTrans = Neb2BtM44Transform(this->transform);
	}
	
	virtual void setWorldTransform(const btTransform& worldTransform)
	{
        this->transform = Bt2NebTransform(worldTransform);
	}

    const Math::matrix44& GetTransform() const;

private:
    Math::matrix44  transform;	
};

//------------------------------------------------------------------------------
/**
*/
inline 
const Math::matrix44& 
NebulaMotionState::GetTransform() const
{
    return this->transform;
}