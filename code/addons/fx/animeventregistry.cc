//------------------------------------------------------------------------------
//  animeventregistry.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "fx/animeventregistry.h"
#include "db/reader.h"
#include "db/dbserver.h"

namespace Attr
{
    // general
    DefineString(AnimEventName, 'aeen', ReadOnly);

    // attachment attrs
    DefineString(Resource, 'aers', ReadOnly);
    DefineString(Joint, 'aejo', ReadOnly);
    DefineFloat(Duration, 'aedu', ReadOnly);
    DefineBool(KeepLocalPosition, 'aekl', ReadOnly);
    DefineString(Rotation, 'aero', ReadOnly);

    // cam shake stuff
    DefineFloat(IntensityX, 'aeix', ReadOnly);
    DefineFloat(IntensityY, 'aeiy', ReadOnly);
    DefineFloat(IntensityZ, 'aeiz', ReadOnly);
    DefineFloat(RotationX, 'aerx', ReadOnly);
    DefineFloat(RotationY, 'aery', ReadOnly);
    DefineFloat(RotationZ, 'aerz', ReadOnly);
    DefineFloat(Range, 'aera', ReadOnly);

    // sound stuff
    DefineString(EventName, 'aeev', ReadOnly);
	DefineString(EventProjectId, 'aeep', ReadOnly);

    // timer stuff
    DefineFloat(TimeFactor, 'aetf', ReadOnly);
    DefineFloat(Attack, 'aeat', ReadOnly);
    DefineFloat(Sustain, 'aess', ReadOnly);
    DefineFloat(Release, 'aere', ReadOnly);

    // vibration stuff
    DefineFloat(HighFreqDuration, 'aehf', ReadOnly);
    DefineFloat(HighFreqIntensity, 'aehi', ReadOnly);
    DefineFloat(LowFreqDuration, 'aelf', ReadOnly);
    DefineFloat(LowFreqIntensity, 'aeli', ReadOnly);
    DefineInt(PlayerIndex, 'aepi', ReadOnly);
};

namespace FX
{
__ImplementClass(FX::AnimEventRegistry, 'AERE', Core::RefCounted);
__ImplementInterfaceSingleton(FX::AnimEventRegistry);

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
AnimEventRegistry::AnimEventRegistry() :
    isValid(false)
{
    __ConstructInterfaceSingleton;
}

//------------------------------------------------------------------------------
/**
*/
AnimEventRegistry::~AnimEventRegistry()
{
    if (this->IsValid())
    {
        this->Discard();
    }
    __DestructInterfaceSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
AnimEventRegistry::Setup()
{
    n_assert(!this->IsValid());

    this->LoadCamShakeEvents(Util::StringAtom("_Template_AnimEventCamShake"));
    this->LoadSoundEvents(Util::StringAtom("_Template_AnimEventSound"));
    this->LoadTimingEvents(Util::StringAtom("_Template_AnimEventTiming"));
    this->LoadVibrationEvents(Util::StringAtom("_Template_AnimEventVibration"));
    this->LoadAttachmentEvents(Util::StringAtom("_Template_AnimEventEffects"));

    this->isValid = true;
}

//------------------------------------------------------------------------------
/**
*/
void
AnimEventRegistry::Discard()
{
    n_assert(this->IsValid());

    // TODO CLEANUP HERE
    this->isValid = false;
}

//------------------------------------------------------------------------------
/**
*/
bool
AnimEventRegistry::IsValid() const
{
    return this->isValid;
}

//------------------------------------------------------------------------------
/**
*/
void
AnimEventRegistry::LoadAttachmentEvents(const Util::StringAtom &tableName)
{
    // open database
    Ptr<Db::Reader> reader = Db::Reader::Create();
    reader->SetDatabase(Db::DbServer::Instance()->GetStaticDatabase());
    reader->SetTableName(tableName.Value());    
    if (!reader->Open())
    {
        n_error("AnimEventRegistry::LoadAttachmentEvents: failed to load table '%s' in static database", tableName.Value());        
    }

    int numRows = reader->GetNumRows();
    this->animEventAttachments.BeginBulkAdd();
    int index;
    for (index = 0; index < numRows; index++)
    {
        reader->SetToRow(index);

        Util::StringAtom id = reader->GetString(Attr::AnimEventName);
        Util::StringAtom res = reader->GetString(Attr::Resource);
        Util::StringAtom joint = reader->GetString(Attr::Joint);
        bool keepLocal = reader->GetBool(Attr::KeepLocalPosition);
        Util::String rotation = reader->GetString(Attr::Rotation);
        Timing::Time time = reader->GetFloat(Attr::Duration);

        if(!res.IsValid() && !joint.IsValid() && 0 == time)
        {
            continue;
        }

        AttachmentEvent newEffect;
        newEffect.name = id;
        newEffect.resource = res;
        newEffect.jointName = joint;
        newEffect.duration = time;
        newEffect.keepLocal = keepLocal;
        if ("local" == rotation)
        {
            newEffect.rotation = InternalGraphics::AttachmentServer::Local;
        } 
        else if ("world" == rotation)
        {
            newEffect.rotation = InternalGraphics::AttachmentServer::World;
        }
        else if ("entity" == rotation)
        {
            newEffect.rotation = InternalGraphics::AttachmentServer::Entity;
        }
        else
        {
            n_error("AnimEventRegistry::LoadAttachmentEvents: table:'%s' row:'%i' column:'Rotation' unsupported value '%s'!\n", tableName.Value(), index, rotation.AsCharPtr());
        }
        this->animEventAttachments.Add(newEffect.name, newEffect);
    }
    this->animEventAttachments.EndBulkAdd();
    reader->Close();
}

//------------------------------------------------------------------------------
/**
*/
void
AnimEventRegistry::LoadCamShakeEvents(const Util::StringAtom& tableName)
{
    // open database
    Ptr<Db::Reader> reader = Db::Reader::Create();
    reader->SetDatabase(Db::DbServer::Instance()->GetStaticDatabase());
    reader->SetTableName(tableName.Value());    
    if (!reader->Open())
    {
        n_error("AnimEventRegistry::LoadCamShakeEvents: failed to load table '%s' in static database", tableName.Value());        
    }

    int numRows = reader->GetNumRows();
    this->animEventCamShakes.BeginBulkAdd();
    int index;
    for (index = 0; index < numRows; index++)
    {
        reader->SetToRow(index);

        Util::String id = reader->GetString(Attr::AnimEventName);
        Timing::Time duration = reader->GetFloat(Attr::Duration);
        Math::vector intensity(reader->GetFloat(Attr::IntensityX),
                               reader->GetFloat(Attr::IntensityY),
                               reader->GetFloat(Attr::IntensityZ));
        Math::vector rotation(reader->GetFloat(Attr::RotationX),
                              reader->GetFloat(Attr::RotationY),
                              reader->GetFloat(Attr::RotationZ));
        float range = reader->GetFloat(Attr::Range);

        if (0 == duration && 0 == intensity.x() && 0 == intensity.y() && 0 == intensity.z() && 0 == range &&
            0 == rotation.x() && 0 == rotation.y() && 0 == rotation.z())
        {
            continue;
        }

        ShakeEvent newOne;
        newOne.duration = duration;
        newOne.intensity = intensity;
        newOne.rotation = rotation;
        newOne.range = range;
        animEventCamShakes.Add(id, newOne);
    }
    animEventCamShakes.EndBulkAdd();
    reader->Close();
}

//------------------------------------------------------------------------------
/**
*/
void
AnimEventRegistry::LoadSoundEvents(const Util::StringAtom& tableName)
{
    // open database
    Ptr<Db::Reader> reader = Db::Reader::Create();
    reader->SetDatabase(Db::DbServer::Instance()->GetStaticDatabase());
    reader->SetTableName(tableName.Value());    
    if (!reader->Open())
    {
        n_error("AnimEventRegistry::LoadSoundEvents: failed to load table '%s' in static database", tableName.Value());        
    }

    int numRows = reader->GetNumRows();
    this->animEventSounds.BeginBulkAdd();
    int index;
    for (index = 0; index < numRows; index++)
    {
        reader->SetToRow(index);

        Util::String id = reader->GetString(Attr::AnimEventName);
        const Util::String eventName = reader->GetString(Attr::EventName); 		
		const Audio2::FmodEventProjectId eventProjectId = reader->GetString(Attr::EventProjectId);		      

        // check if string is not empty
        if ((eventName.IsEmpty()) || (!eventProjectId.IsValid()))
        {
            continue;
        }

        this->animEventSounds.Add(id, Audio2::FmodEventId(eventProjectId, eventName));
    }
    this->animEventSounds.EndBulkAdd();
    reader->Close();
}

//------------------------------------------------------------------------------
/**
*/
void
AnimEventRegistry::LoadTimingEvents(const Util::StringAtom& tableName)
{
    // open database
    Ptr<Db::Reader> reader = Db::Reader::Create();
    reader->SetDatabase(Db::DbServer::Instance()->GetStaticDatabase());
    reader->SetTableName(tableName.Value());    
    if (!reader->Open())
    {
        n_error("AnimEventRegistry::LoadTimingEvents: failed to load table '%s' in static database", tableName.Value());        
    }

    int numRows = reader->GetNumRows();
    this->animEventTimer.BeginBulkAdd();
    int index;
    for (index = 0; index < numRows; index++)
    {
        reader->SetToRow(index);

        Util::String id = reader->GetString(Attr::AnimEventName);
        float factor = reader->GetFloat(Attr::TimeFactor);
        Timing::Tick fadeIn = Timing::SecondsToTicks(reader->GetFloat(Attr::Attack));
        Timing::Tick duration = Timing::SecondsToTicks(reader->GetFloat(Attr::Sustain));
        Timing::Tick fadeOut = Timing::SecondsToTicks(reader->GetFloat(Attr::Release));

        if (1 == factor && 0 == duration && fadeIn == 0 && fadeOut == 0)
        {
            continue;
        }

        TimingEvent newOne;
        newOne.factor = factor;
        newOne.sustain = duration;
        newOne.release = fadeOut;
        newOne.attack = fadeIn;
        newOne.startTime = -1;
        animEventTimer.Add(id, newOne);
    }
    animEventTimer.EndBulkAdd();
    reader->Close();
}

//------------------------------------------------------------------------------
/**
*/
void
AnimEventRegistry::LoadVibrationEvents(const Util::StringAtom& tableName)
{
    // open database
    Ptr<Db::Reader> reader = Db::Reader::Create();
    reader->SetDatabase(Db::DbServer::Instance()->GetStaticDatabase());
    reader->SetTableName(tableName.Value());    
    if (!reader->Open())
    {
        n_error("AnimEventRegistry::LoadVibrationEvents: failed to load table '%s' in static database", tableName.Value());        
    }

    int numRows = reader->GetNumRows();
    this->animEventVibs.BeginBulkAdd();
    int index;
    for (index = 0; index < numRows; index++)
    {
        reader->SetToRow(index);

        Util::String id = reader->GetString(Attr::AnimEventName);

        Timing::Time highDuration = reader->GetFloat(Attr::HighFreqDuration);
        float highIntensity = reader->GetFloat(Attr::HighFreqIntensity);

        Timing::Time lowDuration = reader->GetFloat(Attr::LowFreqDuration);
        float lowIntensity = reader->GetFloat(Attr::LowFreqIntensity);

        IndexT playerIndex = reader->GetInt(Attr::PlayerIndex);

        if (0 == lowIntensity && 0 == lowDuration && 0 == highDuration && 0 == highIntensity)
        {
            continue;
        }

        VibrationEvent newOne;
        newOne.highFreqDuration = highDuration;
        newOne.lowFreqDuration = lowDuration;
        newOne.highFreqIntensity = highIntensity;
        newOne.lowFreqIntensity = lowIntensity;
        newOne.playerIndex = playerIndex;

        this->animEventVibs.Add(id, newOne);        
    }
    this->animEventVibs.EndBulkAdd();
    reader->Close();
}
} // namespace FX