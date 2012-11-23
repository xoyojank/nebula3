//------------------------------------------------------------------------------
//  ambiencebubbleproperty.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "properties/ambiencebubbleproperty.h"
#include "game/entity.h"
#include "posteffect/posteffectmanager.h"
#include "properties/lightproperty.h"
//#include "msg/EnableAmbienceBubble.h"
//#include "msg/attributesupdated.h"
//#include "gfx2/ngfxserver2.h"
//#include "managers/debugmanager.h"
#include "basegameprotocol.h"

//------------------------------------------------------------------------------
namespace Attr
{
    DefineBool(AmbienceBubbleEnabled, 'ABEN', ReadWrite);    
    DefineInt(AmbienceBubblePriority, 'ABPR', ReadWrite);
    DefineBool(PEDefaultEntity, 'PEDE', ReadWrite);
    DefineStringWithDefault(PEShapeType, 'PEST', ReadWrite, Util::String("Sphere"));
    DefineFloatWithDefault(PEFadeTime, 'PEFT', ReadWrite, 10.0f);
};

namespace GraphicsFeature
{
__ImplementClass(GraphicsFeature::AmbienceBubbleProperty, 'AMBP', Game::Property);

using namespace BaseGameFeature;
//------------------------------------------------------------------------------
/**
*/
AmbienceBubbleProperty::AmbienceBubbleProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
AmbienceBubbleProperty::~AmbienceBubbleProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
AmbienceBubbleProperty::SetupCallbacks()
{
    this->entity->RegisterPropertyCallback(this, RenderDebug);
}

//------------------------------------------------------------------------------
/**
*/
void
AmbienceBubbleProperty::SetupDefaultAttributes()
{
    SetupAttr(Attr::Saturation);
    SetupAttr(Attr::Balance);
    SetupAttr(Attr::BloomScale);
    SetupAttr(Attr::BloomColor);
    SetupAttr(Attr::BrightPassThreshold);
    SetupAttr(Attr::FogNearDist);
    SetupAttr(Attr::FogFarDist);
    SetupAttr(Attr::FogHeight);
    SetupAttr(Attr::FocusDist);
    SetupAttr(Attr::FocusLength);
    SetupAttr(Attr::SkyContrast);
    SetupAttr(Attr::SkyBrightness);
    SetupAttr(Attr::LightIntensity);
    SetupAttr(Attr::LightColor);
    SetupAttr(Attr::LightOppositeColor);
    SetupAttr(Attr::LightAmbient);
    SetupAttr(Attr::BackLightFactor);
    SetupAttr(Attr::LightShadowIntensity);
    SetupAttr(Attr::FogColor);
    SetupAttr(Attr::FogIntensity);
}

//------------------------------------------------------------------------------
/**
*/
void
AmbienceBubbleProperty::OnActivate()
{
    Property::OnActivate();

    // create a posteffect entity and register with the post effect server
    this->postEffectEntity = PostEffect::PostEffectEntity::Create();
    this->postEffectEntity->SetDefaultEntity(this->entity->GetBool(Attr::PEDefaultEntity));
    this->postEffectEntity->SetShapeType(PostEffect::PostEffectEntity::StringToShapeType(this->entity->GetString(Attr::PEShapeType)));
    this->postEffectEntity->SetBlendTime(this->entity->GetFloat(Attr::PEFadeTime));
    this->postEffectEntity->SetTransform(this->entity->GetMatrix44(Attr::Transform));
    this->postEffectEntity->SetEnabled(this->entity->GetBool(Attr::AmbienceBubbleEnabled));
    this->postEffectEntity->SetPriority(this->entity->GetInt(Attr::AmbienceBubblePriority));
    this->UpdatePostEffectEntityFromAttrs();
    PostEffect::PostEffectManager::Instance()->AttachEntity(this->postEffectEntity);
}

//------------------------------------------------------------------------------
/**
*/
void
AmbienceBubbleProperty::OnDeactivate()
{
    PostEffect::PostEffectManager::Instance()->RemoveEntity(this->postEffectEntity);
    this->postEffectEntity = 0;
    Property::OnDeactivate();
}

//------------------------------------------------------------------------------
/**
*/
void
AmbienceBubbleProperty::SetupAcceptedMessages()
{
    this->RegisterMessage(BaseGameFeature::UpdateTransform::Id);
    this->RegisterMessage(AttributesUpdated::Id);
    this->RegisterMessage(BaseGameFeature::EnableAmbienceBubble::Id);
    Property::SetupAcceptedMessages();
}

//------------------------------------------------------------------------------
/**
*/
void
AmbienceBubbleProperty::HandleMessage(const Ptr<Messaging::Message>& msg)
{
    n_assert(msg);
    if (msg->CheckId(BaseGameFeature::UpdateTransform::Id))
    {
        const Ptr<BaseGameFeature::UpdateTransform>& updTransform = msg.cast<BaseGameFeature::UpdateTransform>();
		if(this->postEffectEntity.isvalid())
		{
			this->postEffectEntity->SetTransform(updTransform->GetMatrix());
		}
	}
    else if (msg->CheckId(AttributesUpdated::Id))
    {   		
		// this is usually coming only from the level editor
		this->UpdatePostEffectEntityFromAttrs();   
        // reset post effect system and re-attach our post entity
        PostEffect::PostEffectManager::Instance()->ResetPostEffectSystem();   
        PostEffect::PostEffectManager::Instance()->AttachEntity(this->postEffectEntity);
    }
    
    else if (msg->CheckId(BaseGameFeature::EnableAmbienceBubble::Id))
    { 
        const Ptr<BaseGameFeature::EnableAmbienceBubble>& enable = msg.cast<BaseGameFeature::EnableAmbienceBubble>();
        this->GetEntity()->SetBool(Attr::AmbienceBubbleEnabled, enable->GetEnabled());
		
		if(postEffectEntity.isvalid())
		{
			this->postEffectEntity->SetEnabled(enable->GetEnabled());
		}
    }
    else
    {
        Game::Property::HandleMessage(msg);
    }
}

//------------------------------------------------------------------------------
/**
    This is only executes at activation or when an AttributesUpdated
    message is coming in.
*/
void
AmbienceBubbleProperty::UpdatePostEffectEntityFromAttrs()
{
    PostEffect::PostEffectEntity::ParamSet& params = this->postEffectEntity->Params();
    params.color->SetColorSaturation(this->entity->GetFloat(Attr::Saturation));
    params.color->SetColorBalance(this->entity->GetFloat4(Attr::Balance));
    params.hdr->SetHdrBloomIntensity(this->entity->GetFloat(Attr::BloomScale));
    params.hdr->SetHdrBloomColor(this->entity->GetFloat4(Attr::BloomColor));
    params.hdr->SetHdrBloomThreshold(this->entity->GetFloat(Attr::BrightPassThreshold));
    params.fog->SetFogNearDistance(this->entity->GetFloat(Attr::FogNearDist));
    params.fog->SetFogFarDistance(this->entity->GetFloat(Attr::FogFarDist));
    params.fog->SetFogHeight(this->entity->GetFloat(Attr::FogHeight));
    params.dof->SetFocusDistance(this->entity->GetFloat(Attr::FocusDist));
    params.dof->SetFocusLength(this->entity->GetFloat(Attr::FocusLength));
    params.sky->SetSkyContrast(this->entity->GetFloat(Attr::SkyContrast));
    params.sky->SetSkyBrightness(this->entity->GetFloat(Attr::SkyBrightness));
    float lightIntensity = this->entity->GetFloat(Attr::LightIntensity);
    params.light->SetLightColor(this->entity->GetFloat4(Attr::LightColor) * lightIntensity);
    params.light->SetLightOppositeColor(this->entity->GetFloat4(Attr::LightOppositeColor));
    params.light->SetLightAmbientColor(this->entity->GetFloat4(Attr::LightAmbient));
    params.light->SetBackLightFactor(this->entity->GetFloat(Attr::BackLightFactor));
    params.light->SetLightShadowIntensity(this->entity->GetFloat(Attr::LightShadowIntensity));
    Math::float4 fogColorAndIntensity = this->entity->GetFloat4(Attr::FogColor);
    fogColorAndIntensity = Math::float4(fogColorAndIntensity.x(),
                                        fogColorAndIntensity.y(),
                                        fogColorAndIntensity.z(),
                                        1.0f - this->entity->GetFloat(Attr::FogIntensity));
    params.fog->SetFogColorAndIntensity(fogColorAndIntensity);

    //// handle sky texture
    Util::String skyTexturePath = this->entity->GetString(Attr::SkyTexture);
    skyTexturePath.StripFileExtension();
    if (skyTexturePath != params.sky->GetSkyTexturePath())
    {
        params.sky->SetSkyTexturePath(skyTexturePath);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
AmbienceBubbleProperty::OnRenderDebug()
{
}

} // namespace Properties

