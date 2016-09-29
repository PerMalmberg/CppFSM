#define CATCH_CONFIG_MAIN
#include "Catch/include/catch.hpp"
#include "TemplatedFSM.h"

class TestBaseState : public tfsm::IBaseState
{
public:
    TestBaseState() : IBaseState("TestBaseState")
    {

    }
};


SCENARIO( "Basic FSM" )
{
    GIVEN( "A clean slate" )
    {
        tfsm::FSM<TestBaseState> fsm;

        WHEN( "FSM is created" )
        {
            THEN( "Current state is null" )
            {
                REQUIRE_FALSE( fsm.HasState() );
            }
        }
        AND_WHEN("State is set")
        {
            fsm.SetState( std::make_unique<TestBaseState>());

            THEN( "FSM has state")
            {
                REQUIRE( fsm.HasState() );
                REQUIRE( fsm.GetState().GetName() == "TestBaseState" );
            }
        }
    }
}

