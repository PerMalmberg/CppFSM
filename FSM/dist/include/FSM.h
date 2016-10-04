// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 
// Give credit where credit is due.

#pragma once

#include <stdlib.h>
#include <memory>
#include <vector>
#include <deque>
#include "Enterchain.h"
#include "EventReceiver.h"

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
	virtual ~FSM()
	{
		myCurrent.reset();
	}

	void SetState( std::unique_ptr<FSMBaseState> newState )
	{
		if( myCurrent )
		{
			myCurrent->DoLeave();
		}

		myCurrent = std::move( newState );
		myCurrent->DoEnter();
	}

	bool HasState()
	{
		return myCurrent != nullptr;
	}

	template<typename EventType>
	void Event( std::unique_ptr<EventType> event )
	{
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
	}

private:
	std::unique_ptr<FSMBaseState> myCurrent = nullptr;
};

///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////


}