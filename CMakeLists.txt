cmake_minimum_required(VERSION 3.10)
project(BitBully VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    # Add later again:
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -Wconversion -Wsign-conversion -Wshadow -Wfloat-equal -Wnon-virtual-dtor -Wold-style-cast -fsanitize=address,undefined,leak -flto")
elseif (MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")
endif ()

# Make sure that asserts only are evaluated in Debug-mode:
if (NOT ${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_compile_definitions(NDEBUG)
endif ()

# Added for windows (msvc)
if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()
IF (WIN32)
    message("Hallo, Windows!")
    # set(gtest_force_shared_crt on) # This is important on Windows...
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT") # How to make /MD work? Only works for msvc
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
ENDIF ()


set(BB_PROJECT_INCS .)
set(BB_PROJECT_FILES BitBully.cpp Board.cpp TranspositionTable.cpp)

include_directories(SYSTEM .)

add_library(BitBully STATIC ${BB_PROJECT_FILES})
target_include_directories(BitBully PUBLIC ${BB_PROJECT_INCS})
target_link_libraries(BitBully ${BB_PROJECT_LIBS})

add_subdirectory(thirdParty)
add_subdirectory(google_tests)