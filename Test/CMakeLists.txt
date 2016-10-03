cmake_minimum_required(VERSION 3.6)
project(Test)
include( ../externals/CMakeUtils/CMakeUtils.txt)

if( IS_WINDOWS )
    # Build at warning level 4 on Windows.
    # Treat all warnings as errors
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
else()
    # Build with all warnings
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
endif()

include_directories( ../externals )
include_directories(../FSM/dist/include)

set(SOURCE_FILES
        test.cpp
        EnterLeave/EnterLeaveTestData.h
        EnterLeave/EnterLeaveState.cpp
        EnterLeave/EnterLeaveState.h
        EnterLeave/EnterLeaveDerived.cpp
        EnterLeave/EnterLeaveDerived.h
        EnterLeave/InitialState.h
        EnterLeave/EnterLeaveBaseState.h
        Event/StateListeningToEvents.cpp
        Event/StateListeningToEvents.h
        Event/AddEvent.h
        Event/SubtractEvent.h
        Event/EventBaseState.h
        Event/EventCounter.h)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

