#pragma once
//------------------------------------------------------------------------------
/**
    @class Particles::ParticleSystemNodeInstance
    
    Wraps a ParticleSystemInstance into a ModelNodeInstance.
    
    (C) 2008 Radon Labs GmbH
*/
#include "models/nodes/statenodeinstance.h"
#include "particles/particlesysteminstance.h"

//------------------------------------------------------------------------------
namespace Particles
{
class ParticleSystemNodeInstance : public Models::StateNodeInstance
{
    __DeclareClass(ParticleSystemNodeInstance);
public:
    /// constructor
    ParticleSystemNodeInstance();
    /// destructor
    virtual ~ParticleSystemNodeInstance();

    /// called from ModelEntity::OnRenderBefore
    virtual void OnRenderBefore(IndexT frameIndex, Timing::Time time);
    /// called during visibility resolve
    virtual void OnVisibilityResolve(IndexT resolveIndex, float distanceToViewer);
    /// apply per-instance state prior to rendering
    virtual void ApplyState();
    /// perform rendering
    virtual void Render();

    /// get the node's particle system instance
    const Ptr<ParticleSystemInstance>& GetParticleSystemInstance() const;

protected:
    /// called when attached to ModelInstance
    virtual void Setup(const Ptr<Models::ModelInstance>& inst, const Ptr<Models::ModelNode>& node, const Ptr<Models::ModelNodeInstance>& parentNodeInst);
    /// called when removed from ModelInstance
    virtual void Discard();
    /// render node specific debug shape
    virtual void RenderDebug();
    /// called when the node becomes visible with current time
    virtual void OnShow(Timing::Time time);
    /// called when the node becomes invisible
    virtual void OnHide(Timing::Time time);

    Ptr<ParticleSystemInstance> particleSystemInstance;    

#if !defined(__WII__) && !defined(__PS3__)
    Ptr<CoreGraphics::ShaderVariableInstance> particleEmitterOrientation;
    Ptr<CoreGraphics::ShaderVariableInstance> particleBillboardOriented;
    Ptr<CoreGraphics::ShaderVariableInstance> particleBBoxCenter;
    Ptr<CoreGraphics::ShaderVariableInstance> particleBBoxSize;
#endif
};

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<ParticleSystemInstance>&
ParticleSystemNodeInstance::GetParticleSystemInstance() const
{
    return this->particleSystemInstance;
}

} // namespace Particles
//------------------------------------------------------------------------------
    