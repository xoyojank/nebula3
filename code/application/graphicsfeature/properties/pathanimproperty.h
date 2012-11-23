#pragma once
//------------------------------------------------------------------------------
/**
    @class Properties::PathAnimProperty

    Attach this property to an entity to move the entity along an 
    animation path. Take care that the property won't collide
    with other properties which influence an entitie's position.

    (C) 2008 Radon Labs GmbH
*/
#include "game/property.h"
#include "basegameattr/basegameattributes.h"
#include "coreanimation/animresource.h"
#include "coreanimation/animsamplebuffer.h"

//------------------------------------------------------------------------------
namespace Attr
{
    DeclareString(AnimPath, 'ANPA', ReadWrite);        // filename of animation file
    DeclareBool(AnimRelative, 'ANRE', ReadWrite);      // animate absolute position or from current position
    DeclareBool(AnimLoop, 'ANLO', ReadWrite);          // loop or clamp animations
    DeclareBool(AnimPlaying, 'ANPL', ReadWrite);       // true if animation currently playing    
    DeclareFloat4(AnimPathOffset, 'ANPO', ReadWrite);  // 
};

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class PathAnimProperty : public Game::Property
{
	__DeclareClass(PathAnimProperty);
public:
    /// constructor
    PathAnimProperty();
    /// destructor
    virtual ~PathAnimProperty();
    /// setup callbacks for this property
    virtual void SetupCallbacks();
    /// setup default entity attributes
    virtual void SetupDefaultAttributes();
    /// called from Entity::ActivateProperties()
    virtual void OnActivate();
    /// called from Entity::OnStart()
    virtual void OnStart();
    /// called from Entity::DeactivateProperties()
    virtual void OnDeactivate();
    /// called before movement happens
    virtual void OnMoveBefore();
    /// override to register accepted messages
    virtual void SetupAcceptedMessages();
    /// handle a single message
    virtual void HandleMessage(const Ptr<Messaging::Message>& msg);

protected:
    /// setup animation resource
    bool SetupAnimation(const Util::String& animPath);
    /// discard animation resource
    void DiscardAnimation();
    /// start playback
    virtual void Play();
    /// stop playback
    virtual void Stop();
    /// rewind
    void Rewind();
    /// update game entity by animation
    virtual void UpdateAnimation();

    Math::matrix44 initialTransform;  
    Timing::Time animTime;
    Timing::Time animStartTime; 
    Timing::Time animEndTime;
    Ptr<CoreAnimation::AnimResource> animResource;
    Ptr<CoreAnimation::AnimSampleBuffer> animSampleBuffer;
};
__RegisterClass(PathAnimProperty);

}; // namespace Properties

//------------------------------------------------------------------------------


