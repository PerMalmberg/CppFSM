// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 
// Give credit where credit is due.

#pragma once

#include <stdlib.h>
#include <memory>
#include <vector>
#include <deque>
#include <queue>
#include "EnterChain.h"
#include "LeaveChain.h"
#include "EventReceiver.h"
#include "IFsmLogger.h"
#include "NullLogger.h"
#include "EventBase.h"

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
	typedef std::queue<std::unique_ptr<fsm::EventBase<FSMBaseState>>> EventQueue;
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
			: myLogger( logger ), myEvents()
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


	// This method must be called continuously by the application to keep the FSM alive.
	virtual void Tick()
	{
		if( HasState() )
		{
			if( myEvents.size() > 0 ) {
				auto event = std::move( myEvents.front() );
				myEvents.pop();
				event->Execute( myCurrent );
			}
			else
			{
				myCurrent->Tick();
			}
		}
	}

	// Enqueues an event for processing on the next tick
	void EnqueueEvent( std::unique_ptr<fsm::EventBase<FSMBaseState>> event )
	{
		myEvents.push( std::move( event ) );
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
		// Save a value for alter comparison, see note about Leave() below.
		auto changeCount = myStateChangeCounter;

		if( myCurrent )
		{
			// It is possible, but a very weird use-case, that a state sets a new state in its Leave() method.
			// If this happens, it means that the state overrides any other exist paths.

			// Take local ownership of the current state to prevent further
			// actions on this state. (Reason: see note about Leave() below.
			auto state = std::move( myCurrent );
			myLogger->LeavingState( state->GetName() );
			state->DoLeave();
		}

		// Only set the new state if no state change already has happened prior to this point.
		// For example, a state may change state in its Leave() chain. In these cases
		// we only want the last state in the chain to be set.
		if( changeCount == myStateChangeCounter )
		{
			// Clear any waiting events when changing states.
			// NOTE: If you are considering changing this behaviour you're doing it wrong - what you
			// should do is to initialize your new state with what it needs in its constructor.
			while( !myEvents.empty() ) {
				myEvents.pop();
			}

			// From unique to shared pointer.
			auto s = newState.release();
			myCurrent = std::shared_ptr<FSMBaseState>( s );

			// Initialize the new state
			myCurrent->SetFSM( this );

			// And perform the Enter procedure
			myLogger->EnteringState( myCurrent->GetName() );
			myCurrent->DoEnter();

			// Signal that a new state has been set.
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

	std::shared_ptr<FSMBaseState> myCurrent;
	int32_t myStateChangeCounter = 0;
	std::shared_ptr<IFsmLogger> myLogger;

	EventQueue myEvents;
};

}