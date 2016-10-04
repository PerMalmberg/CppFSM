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
			// This way of calling causes ambiguous method lookup during template instantiation
			//myCurrent->Event( std::move( event ) );

			auto* s = myCurrent.get();
			static_cast<EventReceiver<EventType>*>( s )->Event( std::move( event ) );
		}
	}

private:
	std::unique_ptr<FSMBaseState> myCurrent = nullptr;
};

///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
template <typename FSMBaseState>
class BaseState
{
public:
	BaseState( const std::string name, FSM<FSMBaseState>& fsm ) : myFsm( fsm ), myName( name )
	{}

	virtual ~BaseState()
	{}

	void AddToEnterChain( IEnterChain *chain )
	{
		myEnterChain.push_back( chain );
	}

	void AddToLeaveChain( ILeaveChain *chain )
	{
		myLeaveChain.push_front( chain );
	}

	void DoEnter()
	{
		for( auto state : myEnterChain )
		{
			state->DoEnter();
		}
	}

	void DoLeave()
	{
		for( auto state : myLeaveChain )
		{
			state->DoLeave();
		}
	}

protected:
	FSM<FSMBaseState>& myFsm;
	const std::string myName;
private:
	std::vector<IEnterChain*> myEnterChain;
	std::deque<ILeaveChain*> myLeaveChain;
};

}