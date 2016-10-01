#define CATCH_CONFIG_MAIN

#include "Catch/include/catch.hpp"
#include "FSM.h"

class EnterTestData
{
public:
	int count = 0;
};

///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
class TestState : public fsm::BaseState
{
public:
	TestState( EnterTestData &d, std::string name, fsm::FSM &fsm ) : BaseState( name, fsm ), data( d ),
	                                                                 enterChain( *this ), leaveChain( *this )
	{

	}


protected:
	EnterTestData &data;

private:
	void Enter()
	{
		if( data.count == 0 )
		{
			data.count++;
		}
	}

	void Leave()
	{
		if( data.count == 1 )
		{
			data.count--;
		}
	}

	SetupEnterChain( TestState );
	SetupLeaveChain( TestState );
};

///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
class TestStateDerived : public TestState
{
public:
	TestStateDerived( EnterTestData &d, fsm::FSM &fsm ) : TestState( d, "TestStateDerived", fsm ),
	                                                      enterChain( *this ), leaveChain( *this )
	{

	}

private:
	void Enter()
	{
		if( data.count == 1 )
		{
			data.count++;
		}
	}

	void Leave()
	{
		if( data.count == 2 )
		{
			data.count--;
		}
	}

	SetupEnterChain( TestStateDerived );

	SetupLeaveChain( TestStateDerived );
};

class InitialState : public fsm::BaseState
{
public:
	InitialState( fsm::FSM &fsm ) : BaseState( "InitialState", fsm )
	{

	}
};

SCENARIO( "Basic FSM" )
{
	GIVEN( "A clean slate" )
	{
		fsm::FSM fsm;

		WHEN( "FSM is created" )
		{
			THEN( "Has no state" )
			{
				REQUIRE_FALSE( fsm.HasState());
			}
		}
		AND_WHEN( "State is set" )
		{
			fsm.SetState( std::make_unique<InitialState>( fsm ));

			THEN( "Has state" )
			{
				REQUIRE( fsm.HasState());
			}
		}
	}
}

SCENARIO( "Enter order" )
{
	GIVEN( "A clean slate" )
	{
		fsm::FSM fsm;

		WHEN( "FSM is created" )
		{
			THEN( "Has no state" )
			{
				REQUIRE_FALSE( fsm.HasState());
			}
		}
		AND_WHEN( "State is set" )
		{
			fsm.SetState( std::make_unique<InitialState>( fsm ));

			THEN( "Has state" )
			{
				REQUIRE( fsm.HasState());
			}
		}
		AND_WHEN( "New state is set, enter and leave is called in correct order" )
		{
			EnterTestData d;
			fsm.SetState( std::make_unique<TestStateDerived>( d, fsm ));
			REQUIRE( d.count == 2 );
			fsm.SetState( std::make_unique<InitialState>( fsm ));
			REQUIRE( d.count == 0 );
		}

	}
}
