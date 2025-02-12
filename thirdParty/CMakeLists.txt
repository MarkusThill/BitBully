cmake_minimum_required(VERSION 3.15)
project(third_party)
set(CMAKE_CXX_STANDARD 17)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
elseif (MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")
endif ()

find_library(PONS_C4 NAMES ponsc4)

# Do the following; Otherwise we get strange compile-time errors
# set(gtest_force_shared_crt on)

add_subdirectory(googletest)
add_subdirectory(connect4)

set(TP_PROJECT_LIBS gtest gmock ponsc4) # gmock_main, gtest_main: Only, if no
# own main.cpp is provided for the
# Google Tests

include_directories(. ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR}
        ${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

if (WIN32)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else ()
    # nothing to do yet...
endif ()
add_library(third_party INTERFACE)
target_link_libraries(third_party INTERFACE ${TP_PROJECT_LIBS})
