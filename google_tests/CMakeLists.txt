# 'Google_test' is the subproject name
project(google_tests)
set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
elseif (MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")
endif ()

#cmake_policy(SET CMP0091 NEW)
set(PROJECT_TESTS BoardTest.cpp VerificationTest.cpp BitBullyTest.cpp)

IF (WIN32)
    message("Hallo, Windows!")
    # set(gtest_force_shared_crt on) # This is important on Windows...
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT") # Only works for msvc
ENDIF ()

message("Hallo, rest of world!")

# pthread is required for gtest/gmock
set(PROJECT_LIBS BitBully third_party gtest_main gmock_main gtest gmock)  #  gtest_main: only required, if no main.cpp is used

# 'Google_Tests_run' is the target name
add_executable(Google_Tests_run ${PROJECT_TESTS} ${PROJECT_SOURCES})

target_link_libraries(Google_Tests_run ${PROJECT_LIBS}) #stdc++fs

#target_compile_options(Google_Tests_run PRIVATE "-masm=intel")