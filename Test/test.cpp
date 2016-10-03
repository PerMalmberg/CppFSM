// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 
// Give credit where credit is due. 

#define CATCH_CONFIG_MAIN

#include "Catch/include/catch.hpp"
#include "FSM.h"
#include "EnterLeave/InitialState.h"
#include "EnterLeave/EnterLeaveTestData.h"
#include "EnterLeave/EnterLeaveDerived.h"
#include "Event/StateListeningToEvents.h"

SCENARIO( "Basic FSM" )
{
	GIVEN( "A clean slate" )
	{
		fsm::FSM<EnterLeaveBaseState> fsm;

		WHEN( "FSM is created" )
		{
			THEN( "Has no state" )
			{
				REQUIRE_FALSE( fsm.HasState() );
			}
		}
		AND_WHEN( "State is set" )
		{
			fsm.SetState( std::make_unique<InitialState>( fsm ) );

			THEN( "Has state" )
			{
				REQUIRE( fsm.HasState() );
			}
		}
	}
}

SCENARIO( "Enter leave order" )
{
	GIVEN( "A clean slate" )
	{
		fsm::FSM<EnterLeaveBaseState> fsm{};

		WHEN( "FSM is created" )
		{
			THEN( "Has no state" )
			{
				REQUIRE_FALSE( fsm.HasState() );
			}
		}
		AND_WHEN( "State is set" )
		{
			fsm.SetState( std::make_unique<InitialState>( fsm ) );

			THEN( "Has state" )
			{
				REQUIRE( fsm.HasState() );
			}
		}
		AND_WHEN( "New state is set, enter and leave is called in correct order" )
		{
			EnterLeaveTestData d;
			fsm.SetState( std::make_unique<EnterLeaveDerived>( d, fsm ) );
			REQUIRE( d.count == 2 );
			fsm.SetState( std::make_unique<InitialState>( fsm ) );
			REQUIRE( d.count == 0 );
		}
	}
}

SCENARIO( "Send an event, have state react to it" )
{
	GIVEN( "An FSM with an initial state" )
	{
		fsm::FSM<EventBaseState> fsm;
		EventCounter count;
		fsm.SetState( std::make_unique<StateListeningToEvents>( fsm, count ) );
		REQUIRE( fsm.HasState() );

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

				THEN( "This event is also forwarded to the current state")
				{
					REQUIRE( count.count == 0 );
				}
			}
		}
	}
}