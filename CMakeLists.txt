cmake_minimum_required(VERSION 3.10)
project(BitBully VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif ()

include_directories(SYSTEM .)


# Google Tests..
add_subdirectory(connect4)
include_directories(connect4)
add_subdirectory(google_tests)

add_executable(BitBully main.cpp)