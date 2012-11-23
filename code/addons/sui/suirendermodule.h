#pragma once
//------------------------------------------------------------------------------
/**
    @class SUI::SUIRenderModule

    The central client-side object for the SimpleUI system.

    (C) 2009 Radon Labs GmbH
*/
#include "rendermodules/rendermodule.h"
#include "core/singleton.h"
#include "util/stringatom.h"
#include "resources/resourceid.h"
#include "input/inputevent.h"
#include "sui/suievent.h"
#include "sui/suiactions.h"
#include "sui/suiprotocol.h" 

//------------------------------------------------------------------------------
namespace SUI
{
class SUIRenderModule : public RenderModules::RenderModule
{
    __DeclareClass(SUIRenderModule);
    __DeclareSingleton(SUIRenderModule);
public:
    /// constructor
    SUIRenderModule();
    /// destructor
    virtual ~SUIRenderModule();
    
    /// setup the render module
    virtual void Setup();
    /// discard the render module
    virtual void Discard();
    /// called per-frame (processes input events)
    virtual void OnFrame();

    /// create a layout object
    void CreateLayout(const Util::StringAtom& layoutId, const Resources::ResourceId& resId);
    /// check if a layout has finished loading
    bool IsLayoutLoaded(const Util::StringAtom& layoutId) const;
    /// destroy a layout object
    void DestroyLayout(const Util::StringAtom& layoutId);
    /// show a layout object
    void ShowLayout(const Util::StringAtom& layoutId, int zPrio);
    /// hide a layout object
    void HideLayout(const Util::StringAtom& layoutId);
    /// invoke an action on a layout
    void InvokeAction(const Util::StringAtom& layoutId, const Ptr<SUIAction>& action);

    /// send input events to visible layouts
    void SendInputEvents(const Util::Array<Input::InputEvent>& inputEvents);
    /// check if an SUI event has happened in the current frame
    bool HasEventOccured(const SUIEvent& suiEvent) const;
    /// returns an array of all events that happened in the current frame
    Util::Array<SUIEvent> GetMatchingEvents(const SUIEvent& suiEvent) const;
    
private:
    Util::Array<Input::InputEvent> inputEvents;
    Util::Array<SUIEvent> suiEvents;
    Ptr<SUI::ProcessEvents> processEventsMsg;
    Util::Array<Ptr<SUI::CreateLayout> > pendingCreateLayoutMsgs;
};

} // namespace SUI
//------------------------------------------------------------------------------
