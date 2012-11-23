#pragma once
//------------------------------------------------------------------------------
/**
	@class GraphicsFeature::MayaCameraProperty

    A manually controlled camera property which implements different
    control models.
    
    (C) 2008 Radon Labs GmbH
*/
#include "properties/cameraproperty.h"
#include "renderutil/mayacamerautil.h"
#include "graphicsattr/graphicsattributes.h"

//------------------------------------------------------------------------------
namespace Attr
{
    // maya camera specific attributes
    DeclareFloat4(MayaCameraCenterOfInterest, 'MCCI', ReadWrite);
    DeclareFloat4(MayaCameraDefaultUpVec, 'MCDU', ReadWrite);
};

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class MayaCameraProperty : public CameraProperty
{
	__DeclareClass(MayaCameraProperty);
public:
    /// constructor
    MayaCameraProperty();
    /// destructor
    virtual ~MayaCameraProperty();
    /// called from Entity::ActivateProperties()
    virtual void OnActivate();  
    /// called from Entity::DeactivateProperties()
    virtual void OnDeactivate();
	/// setup callbacks for this property
    virtual void SetupCallbacks();
    /// called before rendering happens
    virtual void OnRender();
    /// setup default entity attributes
    virtual void SetupDefaultAttributes();
    /// override to register accepted messages
    virtual void SetupAcceptedMessages();
    /// handle a single message
    virtual void HandleMessage(const Ptr<Messaging::Message>& msg);
    /// called when input focus is gained
    virtual void OnObtainInputFocus();
    /// called when input focus is lost
    virtual void OnLoseInputFocus();

private:
	RenderUtil::MayaCameraUtil mayaCameraUtil;

};
__RegisterClass(MayaCameraProperty);
}; // namespace GraphicsFeature
//------------------------------------------------------------------------------
   