//------------------------------------------------------------------------------
//  state.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "scriptfeature/fsm/state.h"
#include "scriptfeature/fsm/transition.h"
#include "messaging/message.h"
#include "scriptfeature/actions/behaviour/fsmaction.h"
#include "scriptfeature/actions/sequenceaction.h"
#include "scriptfeature/actions/actionlist.h"
#include "scriptfeature/actions/ifthenelseaction.h"

namespace FSM
{
__ImplementClass(FSM::State, 'FSMS', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
State::State()
{
}

//------------------------------------------------------------------------------
/**
*/
State::~State()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Executes the entry actions. This method is called once by the state 
    machine when the state becomes the active state.
*/
void
State::OnEntry()
{       
    // two functions with the same goal
    this->entryActions->OnActivate();
    this->entryActions->Execute();
    //this->entryActions->Trigger();

    this->frameActions->OnActivate();
 
    // call OnEntry() on all Transitions
    int i;
    for (i = 0; i < this->transitions.Size(); i++)
    {
        this->transitions[i]->OnActivate();
    }     
}

//------------------------------------------------------------------------------
/**
    Executes the frame actions. This method is called once per frame by
    the state machine while this state is the active state.
*/
void
State::OnFrame()
{         
    // trigger the sequence action, this will execute the subsequent actions only once,
    // we need to restart them after they finished.
    // .. which leads to some serious bug [np] -> fsm actions are finished instantly
    // and now get reseted (via OnActivate) each frame *outsch*
    // solution: continuous action reset themself after finishing
    // -> make sure to clean them if you dont want to restart them!
    if (!this->frameActions->Trigger())
    {
        //this->frameActions->OnActivate();
    } 
}

//------------------------------------------------------------------------------
/**
    Executes the exit actions. This method is called once per frame by
    the state machine before this state becomes inactive.
*/
void
State::OnExit()
{       
    // call OnDectivate() on all FrameActions
    this->frameActions->OnDeactivate();
    
    // call OnExit on all Transitions
    int i;
    for (i = 0; i < this->transitions.Size(); i++)
    {
        this->transitions[i]->OnDeactivate();
    }
     
    this->exitActions->Execute();
}

//------------------------------------------------------------------------------
/**
    This evaluates all transitions. The first which evaluates to true will
    be returned, or 0 if no transition evaluates to true. The caller
    must care about executing the transition actions and switching states.
*/
Ptr<Transition>
State::EvaluateTransitions()
{        
    int i;
    for (i = 0; i < this->transitions.Size(); i++)
    {
        if (this->transitions[i]->EvaluateConditions())
        {
             return this->transitions[i];
        }
    } 
    return 0;
}

//------------------------------------------------------------------------------
/**
    Notifies the state about any incoming messages. The method call
    is simply forwarded to all FrameActions and Transitions. Usually, only
    FSMActions and FSMConditions are interested about messages.
*/
void
State::Notify(const Ptr<Messaging::Message>& msg)
{
    this->frameActions->Notify(msg);

    // notify transitions
    int i;
    for (i = 0; i < this->transitions.Size(); i++)
    {
        this->transitions[i]->Notify(msg);
    }
}

//------------------------------------------------------------------------------
/**
*/
int
State::GetNumTransitions() const
{
    return this->transitions.Size();
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<Transition>&
State::GetTransitionAtIndex(int index) const
{
    return this->transitions[index];
}

//------------------------------------------------------------------------------
/**
*/
void
State::SetName(const Util::String& n)
{
    this->name = n;
}

//------------------------------------------------------------------------------
/**
*/
const Util::String&
State::GetName() const
{
    return this->name;
}

//------------------------------------------------------------------------------
/**
*/
void
State::AddEntryAction(const Ptr<Actions::ActionList>& action)
{
    n_assert(0 != action);
    this->entryActions = Actions::SequenceAction::Create();
    this->entryActions->FromList(action.cast<Actions::Action>());
}

//------------------------------------------------------------------------------
/**
*/
void
State::AddFrameAction(const Ptr<Actions::ActionList>& action)
{
    n_assert(0 != action);
    this->frameActions = Actions::SequenceAction::Create();
    this->frameActions->FromList(action.cast<Actions::Action>());
}

//------------------------------------------------------------------------------
/**
*/
void
State::AddExitAction(const Ptr<Actions::ActionList>& action)
{
    n_assert(0 != action);
    this->exitActions = Actions::SequenceAction::Create();
    this->exitActions->FromList(action.cast<Actions::Action>());
}

//------------------------------------------------------------------------------
/**
*/
void
State::AddTransition(const Ptr<Transition>& transition)
{
    n_assert(0 != transition);
    this->transitions.Append(transition);
}

}; // namespace FSM
