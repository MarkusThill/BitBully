cmake_minimum_required(VERSION 3.10)
project(BitBully VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

include_directories(SYSTEM .)

add_executable(BitBully main.cpp)

# Google Tests..
add_subdirectory(google_tests)