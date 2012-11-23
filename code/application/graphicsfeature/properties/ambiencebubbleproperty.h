#pragma once
//------------------------------------------------------------------------------
/**
    @class Properties::AmbienceBubbleProperty
  
    Implement a postprocessing "ambience bubble" in the world which
    changes several postprocessing attributes when the player
    enters it.
    
    (C) 2006 Radon Labs GmbH
*/
#include "game/property.h"
#include "attr/attribute.h"
#include "posteffect/posteffectentity.h"

//------------------------------------------------------------------------------
namespace Attr
{
    DeclareBool(AmbienceBubbleEnabled, 'ABEN', ReadWrite);
    DeclareInt(AmbienceBubblePriority, 'ABPR', ReadWrite);
    DeclareBool(PEDefaultEntity, 'PEDE', ReadWrite);
    DeclareString(PEShapeType, 'PEST', ReadWrite);
    DeclareFloat(PEFadeTime, 'PEFT', ReadWrite);
};

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class AmbienceBubbleProperty : public Game::Property
{
    __DeclareClass(AmbienceBubbleProperty);
public:
    /// constructor
    AmbienceBubbleProperty();
    /// destructor
    virtual ~AmbienceBubbleProperty(); 

    /// setup callbacks for this property, call by entity in OnActivate()
    virtual void SetupCallbacks();
    /// setup default entity attributes
    virtual void SetupDefaultAttributes();
    /// called from Entity::ActivateProperties()
    virtual void OnActivate();
    /// called from Entity::DeactivateProperties()
    virtual void OnDeactivate();
    /// called when game debug visualization is on
    virtual void OnRenderDebug();
    /// override to register accepted messages
    virtual void SetupAcceptedMessages();
    /// handle a single message
    virtual void HandleMessage(const Ptr<Messaging::Message>& msg);

private:
    /// update the posteffect property from our entity attributes
    void UpdatePostEffectEntityFromAttrs();

    Ptr<PostEffect::PostEffectEntity> postEffectEntity;
};
__RegisterClass(AmbienceBubbleProperty);

} // namespace Properties
//------------------------------------------------------------------------------


