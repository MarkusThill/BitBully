cmake_minimum_required(VERSION 3.15)
project(gtests)
set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
elseif (MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")
endif ()

# cmake_policy(SET CMP0091 NEW)
set(PROJECT_TESTS BoardTest.cpp VerificationTest.cpp BitBullyTest.cpp
        OpeningBookTests.cpp)

if (WIN32)
    message("Hallo, Windows!")
    # set(gtest_force_shared_crt on) # This is important on Windows...
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT") # Only works for
    # msvc
endif ()

message("Hallo, rest of world!")

# pthread is required for gtest/gmock
set(PROJECT_LIBS cppBitBully third_party gtest_main gmock_main gtest gmock
)# gtest_main: only required, if no main.cpp is used

# 'Google_Tests_run' is the target name
add_executable(gtest_run ${PROJECT_TESTS} ${PROJECT_SOURCES})

target_link_libraries(gtest_run ${PROJECT_LIBS}) # stdc++fs

# target_compile_options(Google_Tests_run PRIVATE "-masm=intel")

# Get Targets in ${CMAKE_CURRENT_SOURCE_DIR}
get_property(
        current_targets
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        PROPERTY BUILDSYSTEM_TARGETS)
message("Current targets: ${current_targets}")
foreach (TARGET ${current_targets})
    set_target_properties(
            ${TARGET}
            PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/gtests/$<0:>
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/gtests/$<0:>
            ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/gtests/$<0:>)
endforeach ()
