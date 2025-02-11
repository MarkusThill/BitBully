cmake_minimum_required(VERSION 3.15)

# Needed for std::filesystem, etc.
# Needs to be placed at the very top of the file:
# https://cmake.org/cmake/help/latest/variable/CMAKE_OSX_DEPLOYMENT_TARGET.html
if (APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum macOS deployment target" FORCE)
endif ()

project(BitBully VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

# Why -fPIC?
# -Independent Code allows the shared library to be loaded at any memory address without modification.
# Static libraries not built with -fPIC embed absolute addresses, which cause issues when linking into shared objects.
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # -fPIC flag


if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    # Add later again: set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra
    # -Wpedantic -Werror -Wconversion -Wsign-conversion -Wshadow -Wfloat-equal
    # -Wnon-virtual-dtor -Wold-style-cast -fsanitize=address,undefined,leak
    # -flto")
elseif (MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")
endif ()

# Make sure that asserts only are evaluated in Debug-mode:
if (NOT ${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_compile_definitions(NDEBUG)
endif ()

# Added for windows (msvc)
if (POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif ()
if (WIN32)
    message("Hallo, Windows!")
    # set(gtest_force_shared_crt on) # This is important on Windows...
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT") # How to make
    # /MD work? Only works for msvc
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif ()

set(BB_PROJECT_INCS .)
set(BB_PROJECT_FILES src/BitBully.cpp src/Board.cpp src/TranspositionTable.cpp src/MoveList.cpp src/OpeningBook.cpp)

include_directories(SYSTEM . src)

add_library(cppBitBully STATIC ${BB_PROJECT_FILES})
target_include_directories(cppBitBully PUBLIC ${BB_PROJECT_INCS})
target_link_libraries(cppBitBully ${BB_PROJECT_LIBS})

# Set compiler-specific warning flags (only for our own code)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(cppBitBully PRIVATE -Wall -Wextra -Wpedantic)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Clear existing warning levels
    string(REPLACE "/W3" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REPLACE "/W3" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    string(REPLACE "/W3" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    target_compile_options(cppBitBully PRIVATE /W4)
endif ()

add_subdirectory(thirdParty)
add_subdirectory(gtests)

# bitbully_core:
# --------------
# https://pybind11.readthedocs.io/en/stable/basics.html
# https://pybind11.readthedocs.io/en/stable/compiling.html#compiling
# https://pybind11.readthedocs.io/en/stable/compiling.html#cmake
add_subdirectory(thirdParty/pybind11)
pybind11_add_module(bitbully_core src/bitbully_core.cpp)
target_link_libraries(bitbully_core PUBLIC cppBitBully)
install(TARGETS bitbully_core DESTINATION .)

# bitbully:
# ---------
# Add a new executable target that compiles main.cpp and links cppBitBully
add_executable(bitbully src/main.cpp)

# Link the existing cppBitBully library to the new executable
target_link_libraries(bitbully PRIVATE cppBitBully third_party)

# Ensure it uses the same include directories as cppBitBully
target_include_directories(bitbully PRIVATE ${BB_PROJECT_INCS})
