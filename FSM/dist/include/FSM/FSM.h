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
	typedef std::queue<std::unique_ptr<FSMBaseState>> StateQueue;

	///////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////
	explicit FSM( std::unique_ptr<FSMBaseState> initialState )
			: FSM( std::move( initialState ), std::make_shared<NullLogger>() )
	{
	}

	///////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////
	FSM( std::unique_ptr<FSMBaseState> initialState, std::shared_ptr<IFsmLogger> logger )
			: myLogger( logger ), myEvents(), myStates()
	{
		myStates.push( std::move( initialState ) );
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
	// Either call Run() or continuously call this method until it returns false
	//
	///////////////////////////////////////////////////////////////////////////
	virtual bool Tick()
	{
		if( myContinueToRun )
		{
			if( myStates.size() > 0 )
			{
				auto state = std::move( myStates.front() );
				myStates.pop();
				ActivateState( std::move( state ) );
			}

			// Send events.
			if( HasState() )
			{
				if( myEvents.size() > 0 )
				{
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
		return myContinueToRun;
	}

	///////////////////////////////////////////////////////////////////////////
	//
	// Calls Tick() until a state has called the Terminate() method to terminate
	// the state and event execution.
	//
	///////////////////////////////////////////////////////////////////////////
	void Run()
	{
		while( Tick() ) {}
	}

	///////////////////////////////////////////////////////////////////////////
	//
	// Enqueues an event for processing on the next tick
	//
	///////////////////////////////////////////////////////////////////////////
	void EnqueueEvent( std::unique_ptr<fsm::EventBase<FSMBaseState>> event )
	{
		myEvents.push( std::move( event ) );
	}

	///////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////
	void SetState( std::unique_ptr<FSMBaseState> newState )
	{
		myStates.push( std::move( newState ) );
	}

	///////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////
	void Terminate()
	{
		myContinueToRun = false;
		// When terminating, call the leave-chain for the current state.
		ExecuteLeave();
	}

private:

	///////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////
	void ActivateState( std::unique_ptr<FSMBaseState> newState )
	{
		ExecuteLeave();

		// Clear any waiting events when changing states.
		// NOTE: If you are considering changing this behaviour you're doing it wrong - what you
		// should do is to initialize your new state with what it needs in its constructor.
		while( !myEvents.empty() )
		{
			myEvents.pop();
		}

		// From unique to shared pointer.
		auto s = newState.release();
		myCurrent = std::shared_ptr<FSMBaseState>( s );
		myLogger->ActivatingState( myCurrent->GetName() );

		// Initialize the new state
		myCurrent->SetFSM( this );

		// And perform the Enter procedure
		myLogger->EnteringState( myCurrent->GetName() );
		myCurrent->DoEnter();

		// Signal that a new state has been set.
		myStateChangeCounter++;
	}

	void ExecuteLeave() const
	{
		if( HasState() )
		{
			auto state = move( myCurrent );
			myLogger->LeavingState( state->GetName() );
			state->DoLeave();
		}
	}

	///////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////
	bool HasState() const
	{
		return myCurrent != nullptr;
	}

	std::shared_ptr<FSMBaseState> myCurrent;
	int32_t myStateChangeCounter = 0;
	std::shared_ptr<IFsmLogger> myLogger;

	EventQueue myEvents;
	StateQueue myStates;
	bool myContinueToRun = true;
};

}