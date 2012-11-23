#pragma once
//------------------------------------------------------------------------------
/**
    @class Particles::ParticleSystemNode
  
    The ParticleSystemNode wraps a ParticleSystem object into a 
    ModelNode for rendering.
    
    (C) 2008 Radon Labs GmbH
*/    
#include "models/nodes/statenode.h"
#include "resources/managedmesh.h"
#include "particles/particlesystem.h"
#include "particles/emitterattrs.h"

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

    /// create a model node instance
    virtual Ptr<Models::ModelNodeInstance> CreateNodeInstance() const;
    /// called when attached to model node
    virtual void OnAttachToModel(const Ptr<Models::Model>& model);
    /// called when removed from model node
    virtual void OnRemoveFromModel();
    /// called when resources should be loaded
    virtual void LoadResources();
    /// called when resources should be unloaded
    virtual void UnloadResources();
    /// get overall state of contained resources (Initial, Loaded, Pending, Failed, Cancelled)
    virtual Resources::Resource::State GetResourceState() const;
    /// called once when all pending resource have been loaded
    virtual void OnResourcesLoaded();
    /// parse data tag (called by loader code)
    virtual bool ParseDataTag(const Util::FourCC& fourCC, const Ptr<IO::BinaryReader>& reader);

    /// get the owned ParticleSystem object
    const Ptr<ParticleSystem>& GetParticleSystem() const;
    /// set resource id of emitter mesh
    void SetEmitterMeshResourceId(const Resources::ResourceId& resId);
    /// get emitter mesh resource id
    const Resources::ResourceId& GetEmitterMeshResourceId() const;
    /// set the primitive group index in the emitter mesh
    void SetPrimitiveGroupIndex(IndexT i);
    /// get the primitive group index in the emitter mesh
    IndexT GetPrimitiveGroupIndex() const;
    /// set emitter attributes
    void SetEmitterAttrs(const EmitterAttrs& attrs);
    /// get emitter attributes
    const EmitterAttrs& GetEmitterAttrs() const;

private:
    /// helper function to parse an EnvelopeCurve from a data stream
    EnvelopeCurve ParseEnvelopeCurveData(const Ptr<IO::BinaryReader>& reader) const;

protected:
    Ptr<ParticleSystem> particleSystem;
    EmitterAttrs emitterAttrs;
    Resources::ResourceId meshResId;
    IndexT primGroupIndex;
    Ptr<Resources::ManagedMesh> managedMesh;
};

//------------------------------------------------------------------------------
/**
*/
inline void
ParticleSystemNode::SetEmitterMeshResourceId(const Resources::ResourceId& resId)
{
    this->meshResId = resId;
}//------------------------------------------------------------------------------
/**
*/
inline const Resources::ResourceId&
ParticleSystemNode::GetEmitterMeshResourceId() const
{
    return this->meshResId;
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
inline IndexT
ParticleSystemNode::GetPrimitiveGroupIndex() const
{
    return this->primGroupIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ParticleSystemNode::SetEmitterAttrs(const EmitterAttrs& attrs)
{
    this->emitterAttrs = attrs;
}

//------------------------------------------------------------------------------
/**
*/
inline const EmitterAttrs&
ParticleSystemNode::GetEmitterAttrs() const
{
    return this->emitterAttrs;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<ParticleSystem>&
ParticleSystemNode::GetParticleSystem() const
{
    return this->particleSystem;
}

} // namespace Particles
//------------------------------------------------------------------------------
