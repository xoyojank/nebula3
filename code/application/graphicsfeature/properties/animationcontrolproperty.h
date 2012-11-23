#pragma once
//------------------------------------------------------------------------------
/**
    @class GraphicsFeature::AnimationControlProperty
    
    Graphics property for animated characters.
    
    (C) 2008 Radon Labs GmbH
*/
#include "properties/graphicsproperty.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class AnimationControlProperty : public GraphicsProperty
{
    __DeclareClass(AnimationControlProperty);
public:
    /// setup default entity attributes
    virtual void SetupDefaultAttributes();
    /// override to register accepted messages
    virtual void SetupAcceptedMessages();
    /// handle a single message
    virtual void HandleMessage(const Ptr<Messaging::Message>& msg);

private:
    /// start walk
    void Walk();
    /// stop walk
    void Stop();

    static const IndexT MovementTrackIndex = 0;
};
__RegisterClass(AnimationControlProperty);

} // namespace GraphicsFeature
//------------------------------------------------------------------------------
    