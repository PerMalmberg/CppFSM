// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 
// Give credit where credit is due. 

#define CATCH_CONFIG_MAIN

#include "Catch/include/catch.hpp"
#include "FSM/FSM.h"
#include "EnterLeave/InitialState.h"
#include "EnterLeave/EnterLeaveTestData.h"
#include "EnterLeave/EnterLeaveDerived.h"
#include "EnterLeave/EnterChangesState.h"
#include "FsmTestLogger.h"
#include "EnterLeave/LeaveChangesState.h"
#include "Event/EventBaseState.h"
#include "Event/StateListeningToEvents.h"
#include "Event/StateThatSendsEventOnTick.h"
#include "Event/Events/CountDownOnDestruction.h"

SCENARIO( "Basic FSM" )
{
	GIVEN( "A clean slate" )
	{
		fsm::FSM<EnterLeaveBaseState> fsm{ std::make_unique<InitialState>(), std::make_shared<FsmTestLogger>() };

		THEN( "Has state" )
		{
			REQUIRE( fsm.GetStateName() == "InitialState" );
		}
	}
}

SCENARIO( "Enter leave order" )
{
	GIVEN( "A clean slate" )
	{
		fsm::FSM<EnterLeaveBaseState> fsm{ std::make_unique<InitialState>(), std::make_shared<FsmTestLogger>() };

		WHEN( "New state is set, enter and leave is called in correct order" )
		{
			EnterLeaveTestData d;
			fsm.SetState( std::make_unique<EnterLeaveDerived>( d ) );

			REQUIRE( d.count == 2 );
			REQUIRE( fsm.GetStateName() == "EnterLeaveDerived" );

			fsm.SetState( std::make_unique<InitialState>() );

			REQUIRE( d.count == 0 );
			REQUIRE( fsm.GetStateName() == "InitialState" );
		}
	}
}

SCENARIO( "Enter method causes state change" )
{
	GIVEN( "An FSM" )
	{
		WHEN( "First state is set, it changes state in the Enter method" )
		{
			fsm::FSM<EnterLeaveBaseState> fsm{ std::make_unique<EnterChangesState>(),
			                                   std::make_shared<FsmTestLogger>() };

			THEN( "Verify that the state has changed exactly two times - initial and new state" )
			{
				REQUIRE( fsm.GetStateChangeCounter() == 2 );
				REQUIRE( fsm.GetStateName() == "FinalState" );
			}
		}
	}
}

SCENARIO( "Leave method causes state change" )
{
	GIVEN( "An FSM with a state that changes state on Leave()" )
	{
		fsm::FSM<EnterLeaveBaseState> fsm{ std::make_unique<LeaveChangesState>(), std::make_shared<FsmTestLogger>() };
		EnterLeaveTestData d;

		REQUIRE( fsm.GetStateChangeCounter() == 1 );
		REQUIRE( fsm.GetStateName() == "LeaveChangesState" );

		WHEN( "Attempt to set new state" )
		{
			fsm.SetState( std::make_unique<EnterLeaveDerived>( d ) );

			THEN( "The desired state is replaced with the state set in the Leave() method" )
			{
				REQUIRE( fsm.GetStateChangeCounter() == 2 );
				REQUIRE( fsm.GetStateName() == "FinalState" );
			}
		}
	}
}

SCENARIO( "Send an event, have state react to it" )
{
	GIVEN( "An FSM with an initial state" )
	{
		Counter c;
		fsm::FSM<EventBaseState> fsm{ std::make_unique<StateListeningToEvents>( c ),
		                              std::make_shared<FsmTestLogger>() };

		REQUIRE( fsm.GetStateName() == "StateListeningToEvents" );

		WHEN( "State sends an event to itself" )
		{
			fsm.Tick(); // First for sending the event
			fsm.Tick(); // Second for processing the event

			THEN( "The event is forwarded to the current state" )
			{
				REQUIRE( c.Get() == 1 );
			}
		}
		AND_WHEN( "Events are available and the state changes" )
		{

		}
	}
}

SCENARIO( "When there are multiple events waiting and the state changes, the events are destroyed" )
{
	GIVEN( "An FSM with an initial state" )
	{
		Counter c;
		fsm::FSM<EventBaseState> fsm{ std::make_unique<StateThatSendsEventOnTick>( c ),
		                              std::make_shared<FsmTestLogger>() };

		REQUIRE( fsm.GetStateName() == "StateThatSendsEventOnTick" );

		WHEN( "An event is queued and state changes" )
		{
			fsm.Tick();

			THEN( "The event is destroyed when removed from the queue" )
			{
				REQUIRE( fsm.GetStateName() == "StateThatSendsEventOnTick" );
				REQUIRE( c.Get() == -2 );
			}
		}
	}
}
