//------------------------------------------------------------------------------
//  particlesystemnode.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "particles/particlesystemnode.h"
#include "particles/particlesystemnodeinstance.h"
#include "coregraphics/streammeshloader.h"
#include "resources/resourcemanager.h"

namespace Particles
{
__ImplementClass(Particles::ParticleSystemNode, 'PSND', Models::StateNode);

using namespace Resources;
using namespace CoreGraphics;
using namespace Models;
using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
ParticleSystemNode::ParticleSystemNode() :
    primGroupIndex(InvalidIndex)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ParticleSystemNode::~ParticleSystemNode()
{
    n_assert(!this->managedMesh.isvalid());
    n_assert(!this->particleSystem.isvalid());
}

//------------------------------------------------------------------------------
/**
*/
Ptr<ModelNodeInstance>
ParticleSystemNode::CreateNodeInstance() const
{
    Ptr<ModelNodeInstance> newInst = (ModelNodeInstance*) ParticleSystemNodeInstance::Create();
    return newInst;
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNode::OnAttachToModel(const Ptr<Model>& m)
{
    n_assert(!this->particleSystem.isvalid());
    StateNode::OnAttachToModel(m);

    // create a new particle system object, don't setup it until 
    // the emitter mesh is asynchronously loaded in 
    // OnResourcesLoaded()
    this->particleSystem = ParticleSystem::Create();
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNode::OnRemoveFromModel()
{
    n_assert(this->particleSystem.isvalid());

    // discard particle system object
    if (this->particleSystem.isvalid())
    {
        this->particleSystem->Discard();
        this->particleSystem = 0;
    }

    // call parent class
    StateNode::OnRemoveFromModel();
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNode::LoadResources()
{
    n_assert(!this->managedMesh.isvalid());
    n_assert(this->meshResId.IsValid());

    // setup the emitter mesh as managed resource, don't setup
    // particle system until the emitter mesh is loaded (in
    // OnLoadResources())
    // the emitter mesh will never be rendered and must only be accessed with
    // the CPU, thus create a custom StreamMeshLoader
    Ptr<StreamMeshLoader> loader = StreamMeshLoader::Create();
    loader->SetUsage(VertexBuffer::UsageCpu);
    loader->SetAccess(VertexBuffer::AccessRead);
    this->managedMesh = ResourceManager::Instance()->CreateManagedResource(Mesh::RTTI, 
        this->meshResId, 
        loader.upcast<ResourceLoader>()).downcast<ManagedMesh>();

    // call parent class
    StateNode::LoadResources();
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNode::UnloadResources()
{
    n_assert(this->managedMesh.isvalid());

    // the managed mesh may still be valid if its content hadn't been loaded yet
    if (this->managedMesh.isvalid())
    {
        ResourceManager::Instance()->DiscardManagedResource(this->managedMesh.upcast<ManagedResource>());
        this->managedMesh = 0;
    }

    // call parent class
    StateNode::UnloadResources();
}

//------------------------------------------------------------------------------
/**
*/
Resource::State
ParticleSystemNode::GetResourceState() const
{
    Resource::State state = StateNode::GetResourceState();
    if (this->managedMesh->GetState() > state)
    {
        state = this->managedMesh->GetState();
    }
    return state;
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNode::OnResourcesLoaded()
{
    n_assert(this->managedMesh->GetState() == Resource::Loaded);
    n_assert(!this->particleSystem->IsValid());

    // setup the particle system
    this->particleSystem->Setup(this->managedMesh->GetMesh(), this->primGroupIndex, this->emitterAttrs);

    // call parent class
    StateNode::OnResourcesLoaded();
}

//------------------------------------------------------------------------------
/**
    Helper function for ParseDataTag, parses the data elements of 
    an EnvelopeCurve from the data stream and returns an EnvelopeCurve
    object.
*/
EnvelopeCurve
ParticleSystemNode::ParseEnvelopeCurveData(const Ptr<BinaryReader>& reader) const
{
    float val0    = reader->ReadFloat();
    float val1    = reader->ReadFloat();
    float val2    = reader->ReadFloat();
    float val3    = reader->ReadFloat();
    float keyPos0 = reader->ReadFloat();
    float keyPos1 = reader->ReadFloat();
    float freq    = reader->ReadFloat();
    float amp     = reader->ReadFloat();
    EnvelopeCurve::ModFunc modFunc = (EnvelopeCurve::ModFunc) reader->ReadInt();
    EnvelopeCurve envCurve;
    envCurve.Setup(val0, val1, val2, val3, keyPos0, keyPos1, freq, amp, modFunc);
    return envCurve;
}

//------------------------------------------------------------------------------
/**
*/
bool
ParticleSystemNode::ParseDataTag(const FourCC& fourCC, const Ptr<BinaryReader>& reader)
{
    bool retval = true;
    if (FourCC('EFRQ') == fourCC)
    {
        this->emitterAttrs.SetEnvelope(EmitterAttrs::EmissionFrequency, this->ParseEnvelopeCurveData(reader));
    }
    else if (FourCC('PLFT') == fourCC)
    {
        this->emitterAttrs.SetEnvelope(EmitterAttrs::LifeTime, this->ParseEnvelopeCurveData(reader));
    }
    else if (FourCC('PSMN') == fourCC)
    {
        this->emitterAttrs.SetEnvelope(EmitterAttrs::SpreadMin, this->ParseEnvelopeCurveData(reader));
    }
    else if (FourCC('PSMX') == fourCC)
    {
        this->emitterAttrs.SetEnvelope(EmitterAttrs::SpreadMax, this->ParseEnvelopeCurveData(reader));
    }
    else if (FourCC('PSVL') == fourCC)
    {
        this->emitterAttrs.SetEnvelope(EmitterAttrs::StartVelocity, this->ParseEnvelopeCurveData(reader));
    }
    else if (FourCC('PRVL') == fourCC)
    {
        this->emitterAttrs.SetEnvelope(EmitterAttrs::RotationVelocity, this->ParseEnvelopeCurveData(reader));
    }
    else if (FourCC('PSZE') == fourCC)
    {
        this->emitterAttrs.SetEnvelope(EmitterAttrs::Size, this->ParseEnvelopeCurveData(reader));
    }
    else if (FourCC('PMSS') == fourCC)
    {
        this->emitterAttrs.SetEnvelope(EmitterAttrs::Mass, this->ParseEnvelopeCurveData(reader));
    }
    else if (FourCC('PTMN') == fourCC)
    {
        this->emitterAttrs.SetEnvelope(EmitterAttrs::TimeManipulator, this->ParseEnvelopeCurveData(reader));
    }
    else if (FourCC('PVLF') == fourCC)
    {
        this->emitterAttrs.SetEnvelope(EmitterAttrs::VelocityFactor, this->ParseEnvelopeCurveData(reader));
    }
    else if (FourCC('PAIR') == fourCC)
    {
        this->emitterAttrs.SetEnvelope(EmitterAttrs::AirResistance, this->ParseEnvelopeCurveData(reader));
    }
    else if (FourCC('PRED') == fourCC)
    {
        this->emitterAttrs.SetEnvelope(EmitterAttrs::Red, this->ParseEnvelopeCurveData(reader));
    }
    else if (FourCC('PGRN') == fourCC)
    {
        this->emitterAttrs.SetEnvelope(EmitterAttrs::Green, this->ParseEnvelopeCurveData(reader));
    }
    else if (FourCC('PBLU') == fourCC)
    {
        this->emitterAttrs.SetEnvelope(EmitterAttrs::Blue, this->ParseEnvelopeCurveData(reader));
    }
    else if (FourCC('PALP') == fourCC)
    {
        this->emitterAttrs.SetEnvelope(EmitterAttrs::Alpha, this->ParseEnvelopeCurveData(reader));
    }
    else if (FourCC('PEDU') == fourCC)
    {
        this->emitterAttrs.SetFloat(EmitterAttrs::EmissionDuration, reader->ReadFloat());
    }
    else if (FourCC('PLPE') == fourCC)
    {
        this->emitterAttrs.SetBool(EmitterAttrs::Looping, (1 == reader->ReadInt()));
    }
    else if (FourCC('PACD') == fourCC)
    {
        this->emitterAttrs.SetFloat(EmitterAttrs::ActivityDistance, reader->ReadFloat());
    }
    else if (FourCC('PROF') == fourCC)
    {
        this->emitterAttrs.SetBool(EmitterAttrs::RenderOldestFirst, (1 == reader->ReadInt()));
    }
    else if (FourCC('PBBO') == fourCC)
    {
        // ATTENTION, this is not set correectly to the right shader instance!!!!
        this->emitterAttrs.SetBool(EmitterAttrs::Billboard, (1 == reader->ReadInt()));
    }
    else if (FourCC('PRMN') == fourCC)
    {
        this->emitterAttrs.SetFloat(EmitterAttrs::StartRotationMin, reader->ReadFloat());
    }
    else if (FourCC('PRMX') == fourCC)
    {
        this->emitterAttrs.SetFloat(EmitterAttrs::StartRotationMax, reader->ReadFloat());
    }
    else if (FourCC('PGRV') == fourCC)
    {
        this->emitterAttrs.SetFloat(EmitterAttrs::Gravity, reader->ReadFloat());
    }
    else if (FourCC('PSTC') == fourCC)
    {
        this->emitterAttrs.SetFloat(EmitterAttrs::ParticleStretch, reader->ReadFloat());
    }
    else if (FourCC('PTTX') == fourCC)
    {
        this->emitterAttrs.SetFloat(EmitterAttrs::TextureTile, reader->ReadFloat());
    }
    else if (FourCC('PSTS') == fourCC)
    {
        this->emitterAttrs.SetBool(EmitterAttrs::StretchToStart, (1 == reader->ReadInt()));
    }
    else if (FourCC('PVRM') == fourCC)
    {
        this->emitterAttrs.SetFloat(EmitterAttrs::VelocityRandomize, reader->ReadFloat());
    }
    else if (FourCC('PRRM') == fourCC)
    {
        this->emitterAttrs.SetFloat(EmitterAttrs::RotationRandomize, reader->ReadFloat());
    }
    else if (FourCC('PSRM') == fourCC)
    {
        this->emitterAttrs.SetFloat(EmitterAttrs::SizeRandomize, reader->ReadFloat());
    }
    else if (FourCC('PPCT') == fourCC)
    {
        this->emitterAttrs.SetFloat(EmitterAttrs::PrecalcTime, reader->ReadFloat());
    }
    else if (FourCC('PRRD') == fourCC)
    {
        this->emitterAttrs.SetBool(EmitterAttrs::RandomizeRotation, (1 == reader->ReadInt()));
    }
    else if (FourCC('PSDL') == fourCC)
    {
        this->emitterAttrs.SetInt(EmitterAttrs::StretchDetail, reader->ReadInt());
    }
    else if (FourCC('PVAF') == fourCC)
    {
        this->emitterAttrs.SetBool(EmitterAttrs::ViewAngleFade, (1 == reader->ReadInt()));
    }
    else if (FourCC('PDEL') == fourCC)
    {
        this->emitterAttrs.SetFloat(EmitterAttrs::StartDelay, reader->ReadFloat());
    }
    else if (FourCC('MESH') == fourCC)
    {
        this->SetEmitterMeshResourceId(reader->ReadString());
    }
    else if (FourCC('PGRI') == fourCC)
    {
        this->SetPrimitiveGroupIndex(reader->ReadInt());
    }
    else
    {
        retval = StateNode::ParseDataTag(fourCC, reader);
    }   
    return retval;    
}

} // namespace Particles
