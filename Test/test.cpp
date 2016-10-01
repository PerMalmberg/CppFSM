#define CATCH_CONFIG_MAIN

#include "Catch/include/catch.hpp"
#include "FSM.h"

class A : public fsm::BaseState
{
public:
	A( std::string name, fsm::FSM &fsm ) : BaseState( name, fsm ), enterChain( *this )
	{

	}

	void Enter()
	{
		++count;
	}

	int count = 0;

private:
	fsm::EnterChain<A> enterChain;
};

class B : public A
{
public:
	B( fsm::FSM &fsm ) : A( "B", fsm ), enterChain( *this )
	{

	}

	void Enter()
	{
		++count;
	}

private:
	fsm::EnterChain<B> enterChain;
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


