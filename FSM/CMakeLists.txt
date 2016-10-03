cmake_minimum_required(VERSION 3.6)
project(FSM)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories(dist/include)

set(SOURCE_FILES
        dist/include/FSM.h
        dist/include/Enterchain.h
        dist/include/EventReceiver.h)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

# Output final binaries in the dist folder so they easily can be distributed together with the header files.
set_target_properties(${PROJECT_NAME}
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "dist/bin/${CMAKE_BUILD_TYPE}"
        LIBRARY_OUTPUT_DIRECTORY "dist/bin/${CMAKE_BUILD_TYPE}"
        RUNTIME_OUTPUT_DIRECTORY "dist/bin/${CMAKE_BUILD_TYPE}"
        )