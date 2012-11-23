#pragma once
//------------------------------------------------------------------------------
/**
    @class FSM::State
    
    Implements a state of a state machine.
    
    (C) 2009 Radon Labs GmbH
*/
#include "core/refcounted.h"

namespace Actions
{
    class SequenceAction;
    class ActionList;
}

namespace Messaging
{
    class Message;
}

//------------------------------------------------------------------------------
namespace FSM
{

class Transition;

class State : public Core::RefCounted
{    
    __DeclareClass(State);
public:
    /// constructor
    State();
    /// destructor
    virtual ~State();
    /// set name of the state
    void SetName(const Util::String& n);
    /// get name of the state
    const Util::String& GetName() const;
    /// add a state entry action
    void AddEntryAction(const Ptr<Actions::ActionList>& action);
    /// add a state frame action
    void AddFrameAction(const Ptr<Actions::ActionList>& action);
    /// add a state exit action
    void AddExitAction(const Ptr<Actions::ActionList>& action);
    /// add a transition object to the state
    void AddTransition(const Ptr<Transition>& transition);
    /// called when state is entered
    void OnEntry();
    /// called per-frame while state is active
    void OnFrame();
    /// called when state is left
    void OnExit();
    /// notify the state about incoming message
    void Notify(const Ptr<Messaging::Message>& msg);
    /// evaluate transitions, return first transition which evaluates true
    Ptr<Transition> EvaluateTransitions();
    /// get number of transitions
    int GetNumTransitions() const;
    /// get pointer to transition at index
    const Ptr<Transition>& GetTransitionAtIndex(int index) const;
    /// get all frame actions
    const Ptr<Actions::SequenceAction>& GetFrameActions() const;

private:
 
    Util::String name;
    Ptr<Actions::SequenceAction> entryActions;
    Ptr<Actions::SequenceAction> frameActions;
    Ptr<Actions::SequenceAction> exitActions;
    Util::Array<Ptr<Transition> > transitions; 
};
__RegisterClass(State)

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Actions::SequenceAction>& 
State::GetFrameActions() const
{
    return this->frameActions;
}
}; // namespace FSM
//------------------------------------------------------------------------------