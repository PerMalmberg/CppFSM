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
include_directories( ../dist/include )

set(SOURCE_FILES test.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
