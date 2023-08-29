cmake_minimum_required(VERSION 3.10)
project(BitBully VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif ()

set(BB_PROJECT_INCS .)
set(BB_PROJECT_FILES BitBully.cpp Board.cpp)

include_directories(SYSTEM .)

add_library(BitBully STATIC ${BB_PROJECT_FILES})
target_include_directories(BitBully PUBLIC ${BB_PROJECT_INCS})
target_link_libraries(BitBully ${BB_PROJECT_LIBS})

add_subdirectory(thirdParty)
add_subdirectory(google_tests)

# add_executable(BitBully main.cpp)