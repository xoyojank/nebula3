#pragma once
//------------------------------------------------------------------------------
/**
    @class Properties::LightProperty

    A light property adds a light source object (Graphics::LightEntity) 
    to a game entity.
    
    (C) 2005 Radon Labs GmbH
*/
#include "game/property.h"
#include "graphics/abstractlightentity.h"
#include "basegameattr/basegameattributes.h"
#include "graphicsutil/lightflickerutil.h"

//------------------------------------------------------------------------------
namespace Attr
{
    DeclareInt(LightType, 'LITY', ReadOnly);
    DeclareFloat4(LightColor, 'LCOL', ReadWrite);   
    DeclareFloat4(LightOppositeColor, 'LOCL', ReadWrite);
    DeclareFloat(LightIntensity, 'LINT', ReadWrite);
    DeclareFloat(LightRange, 'LRAN', ReadWrite);    
    DeclareFloat4(LightAmbient, 'LAMB', ReadWrite);
    DeclareBool(LightCastShadows, 'LCAS', ReadWrite);
    DeclareBool(LightFlickerEnable, 'LFLE', ReadWrite);
    DeclareFloat(LightFlickerFrequency, 'LFLF', ReadWrite);
    DeclareFloat(LightFlickerIntensity, 'LFLI', ReadWrite);
	DeclareFloat(LightFlickerPosition, 'LFLP', ReadWrite);
	DeclareFloat(LightConeAngle, 'LCOA', ReadWrite);
};

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class LightProperty : public Game::Property
{
	__DeclareClass(LightProperty);
public:
    /// setup default entity attributes
    virtual void SetupDefaultAttributes();
    /// setup callbacks for this property, call by entity in OnActivate()
    virtual void SetupCallbacks();
    /// called from Entity::ActivateProperties()
    virtual void OnActivate();

    void SetSpotLightTransform();
    /// called from Entity::DeactivateProperties()
    virtual void OnDeactivate();
    /// override to register accepted messages
    virtual void SetupAcceptedMessages();
    /// handle a single message
    virtual void HandleMessage(const Ptr<Messaging::Message>& msg);
    /// called before rendering happens
    virtual void OnRender();

private:
    /// update embedded Nebula light object from attributes
    void UpdateLightFromAttributes();
    /// update the light flicker utility from entity attributes
    void UpdateLightFlickerUtilFromAttributes();
    /// update light entity transform
    void UpdateLightTransform(const Math::matrix44& transform);

    Ptr<Graphics::AbstractLightEntity> lightEntity;
    LightFlickerUtil lightFlickerUtil;
};
__RegisterClass(LightProperty);

}; // namespace GraphicsFeature
//------------------------------------------------------------------------------

