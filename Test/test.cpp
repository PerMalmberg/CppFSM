#define CATCH_CONFIG_MAIN
#include "Catch/include/catch.hpp"
#include "FSM.h"

class TestBaseState : public fsm::IBaseState
{
public:
    TestBaseState( std::string name, fsm::FSM &fsm ) : IBaseState( name, fsm )
    {

    }
};


class InitalState : public TestBaseState
{
public:
    InitalState( fsm::FSM& fsm ) : TestBaseState("InitialState", fsm)
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
                REQUIRE_FALSE( fsm.HasState() );
            }
        }
        AND_WHEN("State is set")
        {
            fsm.SetState(std::make_unique<InitalState>(fsm));

            THEN( "Has state")
            {
                REQUIRE(fsm.HasState());
            }
        }
    }
}

