cmake_minimum_required(VERSION 3.6)
project(Test)
include( ../externals/CMakeUtils/CMakeUtils.txt)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

if( IS_WINDOWS )
    # Build at warning level 4 on Windows.
    # Treat all warnings as errors
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
else()
    # Build with all warnings
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
endif()

include_directories( ../externals )
include_directories( ../FSM/dist/include )

set(SOURCE_FILES
        ../FSM/dist/include/FSM/BaseState.h
        ../FSM/dist/include/FSM/EnterChain.h
        ../FSM/dist/include/FSM/LeaveChain.h
        ../FSM/dist/include/FSM/EventReceiver.h
        ../FSM/dist/include/FSM/FSM.h
        ../FSM/dist/include/FSM/IFsmLogger.h
        ../FSM/dist/include/FSM/NullLogger.h
        test.cpp
        EnterLeave/EnterLeaveTestData.h
        EnterLeave/EnterLeaveState.cpp
        EnterLeave/EnterLeaveState.h
        EnterLeave/EnterLeaveDerived.cpp
        EnterLeave/EnterLeaveDerived.h
        EnterLeave/InitialState.h
        EnterLeave/EnterLeaveBaseState.h
        EnterLeave/LeaveChangesState.cpp
        EnterLeave/LeaveChangesState.h
        EnterLeave/EnterChangesState.cpp
        EnterLeave/EnterChangesState.h
        EnterLeave/FinalState.cpp
        EnterLeave/FinalState.h
        FsmTestLogger.cpp
        FsmTestLogger.h
        Event/EventBaseState.cpp Event/EventBaseState.h Event/StateListeningToEvents.cpp Event/StateListeningToEvents.h Event/Events/EventA.h Event/Counter.h ../FSM/dist/include/FSM/EventBase.h Event/StateThatSendsEventOnTick.cpp Event/StateThatSendsEventOnTick.h Event/Events/CountDownOnDestruction.h)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

