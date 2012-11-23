//------------------------------------------------------------------------------
//  particlesystemnodeinstance.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "particles/particlesystemnodeinstance.h"
#include "particles/particlesystemnode.h"
#include "coregraphics/transformdevice.h"
#include "particles/particlerenderer.h"
#include "coregraphics/shadersemantics.h"

// DEBUG
#include "models/model.h"

namespace Particles
{
__ImplementClass(Particles::ParticleSystemNodeInstance, 'PSNI', Models::StateNodeInstance);

using namespace Models;
using namespace Util;
using namespace CoreGraphics;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
ParticleSystemNodeInstance::ParticleSystemNodeInstance()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ParticleSystemNodeInstance::~ParticleSystemNodeInstance()
{
    n_assert(!this->particleSystemInstance.isvalid());
}    

//------------------------------------------------------------------------------
/**
*/
void 
ParticleSystemNodeInstance::OnVisibilityResolve(IndexT resolveIndex, float distanceToViewer)
{
    // check if node is inside lod distances or if no lod is used
    const Ptr<TransformNode>& transformNode = this->modelNode.downcast<TransformNode>();
    if (transformNode->CheckLodDistance(distanceToViewer))
    {
        this->modelNode->AddVisibleNodeInstance(resolveIndex, this);
        ModelNodeInstance::OnVisibilityResolve(resolveIndex, distanceToViewer);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNodeInstance::Setup(const Ptr<ModelInstance>& inst, const Ptr<ModelNode>& node, const Ptr<ModelNodeInstance>& parentNodeInst)
{
    n_assert(!this->particleSystemInstance.isvalid());

    // up to parent class
    StateNodeInstance::Setup(inst, node, parentNodeInst);

    // setup a new particle system instance
    this->particleSystemInstance = ParticleSystemInstance::Create();
    const Ptr<ParticleSystemNode>& particleSystemNode = node.downcast<ParticleSystemNode>();
    const Ptr<ParticleSystem>& particleSystem = particleSystemNode->GetParticleSystem();
    this->particleSystemInstance->Setup(particleSystem);

#if __PS3__
    const Util::Array<Util::KeyValuePair<Util::StringAtom, Util::Variant> > &shaderParams = particleSystemNode->GetShaderParameter();
    IndexT i;
    int numAnimPhases = -1;
    float animFramesPerSecond = -1.0f;
    for (i = 0; i < shaderParams.Size(); i++)
    {
        if(shaderParams[i].Key() == "ALPHAREF")
        {
            numAnimPhases = shaderParams[i].Value().GetInt();
        }
        else if(shaderParams[i].Key() == "INTENSITY1")
        {
            animFramesPerSecond = shaderParams[i].Value().GetFloat();
        }
    }
    n_assert(-1 != numAnimPhases);
    n_assert(-1.0f != animFramesPerSecond);
    this->particleSystemInstance->SetNumAnimPhases(numAnimPhases);
    this->particleSystemInstance->SetAnimFramesPerSecond(animFramesPerSecond);
#endif

    // setup variables
#if !defined(__WII__) && !defined(__PS3__)
    this->particleEmitterOrientation = this->CreateShaderVariableInstance(ShaderVariable::Semantic(NEBULA3_SEMANTIC_EMITTERTRANSFORM));
    this->particleBillboardOriented = this->CreateShaderVariableInstance(ShaderVariable::Semantic(NEBULA3_SEMANTIC_BILLBOARD));
    this->particleBBoxCenter = this->CreateShaderVariableInstance(ShaderVariable::Semantic(NEBULA3_SEMANTIC_BBOXCENTER));
    this->particleBBoxSize = this->CreateShaderVariableInstance(ShaderVariable::Semantic(NEBULA3_SEMANTIC_BBOXSIZE));
#endif
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNodeInstance::Discard()
{
    n_assert(this->particleSystemInstance.isvalid());

    // discard our particle system instance
    this->particleSystemInstance->Discard();
    this->particleSystemInstance = 0;

    // up to parent-class
    StateNodeInstance::Discard();
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNodeInstance::OnRenderBefore(IndexT frameIndex, Timing::Time time)
{
    // call parent class
    StateNodeInstance::OnRenderBefore(frameIndex, time);    

    this->particleSystemInstance->OnRenderBefore();

    // update particle system with new model transform
    this->particleSystemInstance->SetTransform(this->modelTransform);

    // updating happens in 2 stages:
    // 1) within activity distance: particles are emitted and updated
    // 2) in view volume: particle system is added for rendering
    const point& eyePos = TransformDevice::Instance()->GetInvViewTransform().get_position();
    const point& myPos  = this->modelTransform.get_position();
    float dist = float4(myPos - eyePos).length();
    float activityDist = this->particleSystemInstance->GetParticleSystem()->GetEmitterAttrs().GetFloat(EmitterAttrs::ActivityDistance);
    if (dist <= activityDist)
    {
        // alright, we're within the activity distance, update the particle system
        this->particleSystemInstance->Update(time);

        // check if we're also in the view volume, and if yes, 
        // register the particle system for rendering
        // FIXME: use actual particle bounding box!!!
        const bbox& globalBox = this->particleSystemInstance->GetBoundingBox();
        const matrix44& viewProj = TransformDevice::Instance()->GetViewProjTransform();
        if (ClipStatus::Outside != globalBox.clipstatus(viewProj))
        {
            // yes, we're visible
            ParticleRenderer::Instance()->AddVisibleParticleSystem(this->particleSystemInstance);
        }
        else
        {
            // FIXME DEBUG
            // n_printf("%f: Particle system invisible (clip) %s!\n", time, this->modelNode->GetName().Value());
        }
    }
    else
    {
        // FIXME DEBUG
        //n_printf("Particle system invisible (activity dist) %s!\n", this->modelNode->GetName().Value());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNodeInstance::ApplyState()
{
    StateNodeInstance::ApplyState();

    // set the shader instance variables...
#if !defined(__WII__) && !defined(__PS3__)
    this->particleEmitterOrientation->SetMatrix(this->GetParticleSystemInstance()->GetTransform());
    this->particleEmitterOrientation->Apply();
    this->particleBillboardOriented->SetBool(this->GetParticleSystemInstance()->GetParticleSystem()->GetEmitterAttrs().GetBool(EmitterAttrs::Billboard));
    this->particleBillboardOriented->Apply();
    this->particleBBoxCenter->SetFloat4(this->GetParticleSystemInstance()->GetBoundingBox().center());
    this->particleBBoxCenter->Apply();
    this->particleBBoxSize->SetFloat4(this->GetParticleSystemInstance()->GetBoundingBox().extents());
    this->particleBBoxSize->Apply();
#endif
}

//------------------------------------------------------------------------------
/**
*/
void 
ParticleSystemNodeInstance::Render()
{
    StateNodeInstance::Render();
    ParticleRenderer::Instance()->RenderParticleSystem(this->particleSystemInstance);
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNodeInstance::RenderDebug()
{
    StateNodeInstance::RenderDebug();
    this->particleSystemInstance->RenderDebug();
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNodeInstance::OnShow(Timing::Time time)
{
    this->particleSystemInstance->Start();
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNodeInstance::OnHide(Timing::Time time)
{
    // FIXME: should stop immediately?
    if (this->particleSystemInstance->IsPlaying())
    {
        this->particleSystemInstance->Stop();
    }
}

} // namespace Particles
