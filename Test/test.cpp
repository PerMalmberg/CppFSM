// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 
// Give credit where credit is due. 

#define CATCH_CONFIG_MAIN

#include "Catch/include/catch.hpp"
#include "FSM.h"
#include "EnterLeave/InitialState.h"
#include "EnterLeave/EnterLeaveTestData.h"
#include "EnterLeave/EnterLeaveDerived.h"


SCENARIO( "Basic FSM" )
{
	GIVEN( "A clean slate" )
	{
		fsm::FSM fsm;

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
		fsm::FSM fsm;

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
