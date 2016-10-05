// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 
// Give credit where credit is due.

#pragma once

#include <stdlib.h>
#include <memory>
#include <vector>
#include <deque>
#include "EnterChain.h"
#include "LeaveChain.h"
#include "EventReceiver.h"
#include "IFsmLogger.h"
#include "NullLogger.h"

namespace fsm {

#define SetupEnterChain( clazz ) \
    const fsm::EnterChain<clazz> enterChain; \
    friend class fsm::EnterChain<clazz>

#define SetupLeaveChain( clazz ) \
    const fsm::LeaveChain<clazz> leaveChain; \
    friend class fsm::LeaveChain<clazz>

///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
template<typename FSMBaseState>
class FSM
{
public:
	///////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////
	FSM( std::unique_ptr<FSMBaseState> initialState )
			: FSM( initialState, std::make_shared<NullLogger>() )
	{
	}

	///////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////
	FSM( std::unique_ptr<FSMBaseState> initialState, std::shared_ptr<IFsmLogger> logger )
			: myLogger( logger )
	{
		SetState( std::move( initialState ) );
	}

	///////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////
	virtual ~FSM()
	{
		myCurrent.reset();
	}

	///////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////
	int32_t GetStateChangeCounter() const
	{
		return myStateChangeCounter;
	}

	///////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////
	std::string GetStateName()
	{
		return HasState() ? myCurrent->GetName() : "";
	}

	///////////////////////////////////////////////////////////////////////////
	//
	// Sends an event to the current state.
	//
	// Returns a State telling if the event caused a state change.
	// NOTE: If the state changed, this means that the previous state has been
	// destroyed and the callee must not access anything outside its current
	// stack frame (i.e. member variables).
	///////////////////////////////////////////////////////////////////////////
	template<typename EventType>
	State Event( std::unique_ptr<EventType> event )
	{
		auto startCount = myStateChangeCounter;

		if( HasState() )
		{
			// Are you getting an ambiguous call error?
			// Here is likely why:
			// A 'using EventReceiver<EventType>::Event;' or an implementation of the Event<T>()
			// method (possibly pure virtual) for each event type must be added to the first class in the
			// inheritance chain from BaseState because inherited methods are not part of the overload lookup.
			// SO question specific to this implementation: http://stackoverflow.com/questions/39845205/template-instantiation-ambiguity
			myCurrent->Event( std::move( event ) );
		}

		// If the event triggered a state change, make sure to tell the callee that.
		return startCount == myStateChangeCounter ? STABLE : CHANGED;
	}

	///////////////////////////////////////////////////////////////////////////
	//
	// This methods sets the current state.
	// Only meant to be called from a class inheriting from FSMBaseState,
	// i.e. not from outside a state.
	//
	///////////////////////////////////////////////////////////////////////////
	void SetState( std::unique_ptr<FSMBaseState> newState )
	{
		auto changeCount = myStateChangeCounter;

		if( myCurrent )
		{
			// It is possible, but a very weird use-case, that a state sets a new state in its Leave() method.
			// If this happens, it means that the state overrides any other exist paths.

			// Take local ownership of the current state to prevent further actions on this state.
			auto state = std::move( myCurrent );
			myLogger->LeavingState( state->GetName() );
			state->DoLeave();
		}

		// Only set the new state if no state change already has happened prior to this point.
		// For example, a state may change state in its Enter() or Leave() chain. In these cases
		// we only want the last state in the chain to be set.
		if( changeCount == myStateChangeCounter )
		{
			myCurrent = std::move( newState );
			myCurrent->SetFSM( this );
			myLogger->EnteringState( myCurrent->GetName() );
			myCurrent->DoEnter();
			myStateChangeCounter++;
		}
	}

private:
	///////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////
	bool HasState()
	{
		return myCurrent != nullptr;
	}


	std::unique_ptr<FSMBaseState> myCurrent;
	int32_t myStateChangeCounter = 0;
	std::shared_ptr<IFsmLogger> myLogger;
};

}