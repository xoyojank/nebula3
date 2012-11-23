#pragma once
//------------------------------------------------------------------------------
/**
    @class Particles::ParticleSystemNode
  
    The ParticleSystemNode wraps a ParticleSystem object into a 
    ModelNode for rendering.
    
    (C) 2008 Radon Labs GmbH
*/    
#include "mayacore/statenode.h"
#include "mayacore/emitterattrs.h"

//------------------------------------------------------------------------------
namespace Particles
{
class ParticleSystemNode : public Models::StateNode
{
    __DeclareClass(ParticleSystemNode);
public:
    /// constructor
    ParticleSystemNode();
    /// destructor
    virtual ~ParticleSystemNode();

	// write data to stream
	virtual bool WriteDataTag(Ptr<ToolkitUtil::ModelWriter>& writer);

    /// set resource id of emitter mesh
	void SetEmitterMeshResourceId(const Util::String& resId);
    /// set the primitive group index in the emitter mesh
    void SetPrimitiveGroupIndex(IndexT i);
    /// set emitter attributes
    void SetEmitterAttrs(const EmitterAttrs& attrs);

protected:
    EmitterAttrs emitterAttrs;
	Util::String meshResId;
    IndexT primGroupIndex;
};

//------------------------------------------------------------------------------
/**
*/
inline void
ParticleSystemNode::SetEmitterMeshResourceId(const Util::String& resId)
{
    this->meshResId = resId;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ParticleSystemNode::SetPrimitiveGroupIndex(IndexT index)
{
    this->primGroupIndex = index;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ParticleSystemNode::SetEmitterAttrs(const EmitterAttrs& attrs)
{
    this->emitterAttrs = attrs;
}

} // namespace Particles
//------------------------------------------------------------------------------
