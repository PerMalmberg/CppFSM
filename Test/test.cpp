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
#include "EnterLeave/LeaveChangesState.h"
#include "FsmTestLogger.h"

SCENARIO( "Basic FSM" )
{
	GIVEN( "A clean slate" )
	{

		fsm::FSM<EnterLeaveBaseState> fsm;
		fsm.SetLogger( std::make_shared<FsmTestLogger>() );

		WHEN( "FSM is created" )
		{
			THEN( "Has no state" )
			{
				REQUIRE( fsm.GetStateName() == "" );
			}
		}
		AND_WHEN( "State is set" )
		{
			fsm.SetState( std::make_unique<InitialState>( fsm ) );

			THEN( "Has state" )
			{
				REQUIRE( fsm.GetStateName() == "InitialState" );
			}
		}
	}
}

SCENARIO( "Enter leave order" )
{
	GIVEN( "A clean slate" )
	{
		fsm::FSM<EnterLeaveBaseState> fsm;
		fsm.SetLogger( std::make_shared<FsmTestLogger>() );

		WHEN( "FSM is created" )
		{
			THEN( "Has no state" )
			{
				REQUIRE( fsm.GetStateName() == "" );
			}
		}
		AND_WHEN( "State is set" )
		{
			fsm.SetState( std::make_unique<InitialState>( fsm ) );

			THEN( "Has state" )
			{
				REQUIRE( fsm.GetStateName() == "InitialState" );
			}
		}
		AND_WHEN( "New state is set, enter and leave is called in correct order" )
		{
			EnterLeaveTestData d;
			fsm.SetState( std::make_unique<EnterLeaveDerived>( d, fsm ) );
			REQUIRE( d.count == 2 );
			REQUIRE( fsm.GetStateName() == "EnterLeaveDerived" );

			fsm.SetState( std::make_unique<InitialState>( fsm ) );
			REQUIRE( d.count == 0 );
			REQUIRE( fsm.GetStateName() == "InitialState" );
		}
	}
}

SCENARIO( "Enter method causes state change" )
{
	GIVEN( "An FSM" )
	{
		fsm::FSM<EnterLeaveBaseState> fsm{ std::make_shared<FsmTestLogger>() };

		WHEN( "First state is set, it changes state in the Enter method" )
		{
			fsm.SetState( std::make_unique<EnterChangesState>( fsm ) );

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
		fsm::FSM<EnterLeaveBaseState> fsm{ std::make_shared<FsmTestLogger>() };
		EnterLeaveTestData d;

		fsm.SetState( std::make_unique<LeaveChangesState>( fsm ) );
		REQUIRE( fsm.GetStateChangeCounter() == 1 );
		REQUIRE( fsm.GetStateName() == "LeaveChangesState" );

		WHEN( "Attempt to set new state" )
		{
			fsm.SetState( std::make_unique<EnterLeaveDerived>( d, fsm ) );

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
		fsm::FSM<EventBaseState> fsm{ std::make_shared<FsmTestLogger>() };
		EventCounter count;
		fsm.SetState( std::make_unique<StateListeningToEvents>( fsm, count ) );
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
		fsm::FSM<EventBaseState> fsm{ std::make_shared<FsmTestLogger>() };
		EventCounter count;
		fsm.SetState( std::make_unique<StateListeningToEvents>( fsm, count ) );
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


