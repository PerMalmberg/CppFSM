// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 
// Give credit where credit is due. 

#define CATCH_CONFIG_MAIN

#include "Catch/include/catch.hpp"
#include "FSM/FSM.h"
#include "Event/Events.h"
#include "EnterLeave/InitialState.h"
#include "EnterLeave/EnterLeaveTestData.h"
#include "EnterLeave/EnterLeaveDerived.h"
#include "Event/StateListeningToEvents.h"
#include "EnterLeave/EnterChangesState.h"
#include "FsmTestLogger.h"
#include "EnterLeave/LeaveChangesState.h"
#include "StateDriven/StateDrivenBaseState.h"
#include "StateDriven/AState.h"

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
		EventCounter count;
		fsm::FSM<EventBaseState> fsm{ std::make_unique<StateListeningToEvents>( count ),
		                              std::make_shared<FsmTestLogger>() };

		REQUIRE( fsm.GetStateName() == "StateListeningToEvents" );

		WHEN( "An event is sent to the FSM" )
		{
			fsm.Event( std::make_unique<AddEvent>() );

			THEN( "The event is forwarded to the current state" )
			{
				REQUIRE( count.count == 1 );
			}
			AND_THEN( "Another event is sent" )
			{
				fsm.Event( std::make_unique<SubtractEvent>() );
				REQUIRE( count.count == 0 );
			}
		}
	}
}

SCENARIO( "Event causes state change" )
{
	GIVEN( "An FSM with an initial state" )
	{
		EventCounter count;
		fsm::FSM<EventBaseState> fsm{ std::make_unique<StateListeningToEvents>( count ),
		                              std::make_shared<FsmTestLogger>() };

		REQUIRE( fsm.GetStateName() == "StateListeningToEvents" );

		WHEN( "An event is sent to the FSM, asking to change state" )
		{
			THEN( "The event is forwarded to the current state" )
			{
				REQUIRE( fsm::CHANGED == fsm.Event( std::make_unique<ChangeStateEvent>() ) );
				REQUIRE( fsm.GetStateName() == "FinalEventState" );
			}
		}
	}
}

SCENARIO( "FSM is state driven" )
{
	GIVEN( "An FSM with initial state" )
	{
		EventCounter c{};
		fsm::FSM<StateDrivenBaseState> fsm{ std::make_unique<AState>( c ), std::make_shared<FsmTestLogger>() };
		REQUIRE( c.count == 2 );

		/*
		 * A, sends GoToBEvent ->
		 *      B, sends GoToCEvent ->
		 *          C, sets state ->
		 *              D, if( count > 5 ) {
		 *                  Stop();
		 *              }
		 *              else {
		 *                  set state A
		 *              }
		 *
		 * */

	}
}
