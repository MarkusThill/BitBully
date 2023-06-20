# 'Google_test' is the subproject name
project(google_tests)
set(CMAKE_CXX_STANDARD 17)

#cmake_policy(SET CMP0091 NEW)

set(PROJECT_SOURCES 4inarow.cpp)
set(PROJECT_TESTS VerificationTest.cpp )

IF (WIN32)
    message("Hallo, Windows!")
    set(gtest_force_shared_crt on) # This is important on Windows...

    # If you have a local lib folder containing gtest/gmock...
    # 'lib' is the folder with Google Test sources
    add_subdirectory(lib)
    include_directories(SYSTEM
            ${gtest_SOURCE_DIR}/include
            ${gtest_SOURCE_DIR}
            ${gmock_SOURCE_DIR}/include
            ${gmock_SOURCE_DIR}
            ..
            )

    #link_directories(../onnxruntime/lib)
    #SET(ONNX_PATH ../onnxruntime/lib)
    #find_library(ONNX_LIB NAMES onnxruntime PATHS ${ONNX_PATH} NO_DEFAULT_PATH)

    set(PROJECT_LIBS gtest gmock gmock_main gtest_main) # gtest_main: only needed when no own main.cpp is used...

    #if (ONNX_LIB AND USE_ONNX_RUNTIME)
    #    message("onnxruntime library was found: ${ONNX_LIB}")
    #    list(APPEND PROJECT_SOURCES ../ONNXEngine.cpp)
    #    list(APPEND PROJECT_TESTS ONNXAutoEncoderTest.cpp)
    #    list(APPEND PROJECT_LIBS onnxruntime)
    #else ()
    #    message("onnxruntime library was NOT found. NOT linking ONNX runtime!")
    #endif ()


ELSE ()
    message("Hallo, rest of world!")
    # If you have a local lib folder containing gtest/gmock...
    # 'lib' is the folder with Google Test sources
    # add_subdirectory(lib)
    include_directories(SYSTEM ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

    # If you have gtest/gmock installed globally...
    include_directories(/usr/src/googletest/googletest/include /usr/src/googletest/googlemock/include)
    find_library(GTEST_LIB NAMES gtest)
    find_library(GMOCK_LIB NAMES gmock)
    find_library(GTEST_MAIN_LIB NAMES gtest_main)
    find_library(GMOCK_MAIN_LIB NAMES gmock_main)

    # add extra include directories
    include_directories(SYSTEM .)
    #include_directories(SYSTEM .
    #        ../dataProcessing
    #        ../isolatedEnv
    #        ../libGlobal
    #        /usr/src/tensorrt/samples/common
    #        /usr/local/cuda/include
    #        /home/mthill/onnxruntime/include/onnxruntime/core/session)
    #link_directories(#/usr/local/cuda/lib64
    #        /home/mthill/onnxruntime/build/Linux/RelWithDebInfo
            #/usr/lib/gcc/aarch64-linux-gnu/8
            #/usr/lib/gcc/aarch64-linux-gnu/7
            #/usr/lib/gcc/aarch64-linux-gnu/10)

    #SET(ONNX_PATH /home/mthill/onnxruntime/build/Linux/RelWithDebInfo)
    #find_library(ONNX_LIB NAMES onnxruntime PATHS ${ONNX_PATH} NO_DEFAULT_PATH)
    #find_library(TRT_LIB NAMES nvinfer)

    #find_library(FS_LIB NAMES stdc++fs)
    #if(NOT FS_LIB)
    #    message("Did not find stdc++fs Library!")
    #else()
    #    message("Filesystem library was found: ${FS_LIB}")
    #endif()

    if ((NOT GTEST_LIB) OR (NOT GTEST_MAIN_LIB))#
        message("ERROR: Did not find gtest/gtest_main. Sure it is installed?")
    endif ()

    # pthread is required for gtest/gmock
    set(PROJECT_LIBS gtest_main gmock_main gtest gmock pthread) #  gtest_main: only required, if no main.cpp is used

    #if (ONNX_LIB AND USE_ONNX_RUNTIME)
    #    message("onnxruntime library was found: ${ONNX_LIB}")
    #    list(APPEND PROJECT_TESTS ONNXAutoEncoderTest.cpp)
    #    list(APPEND PROJECT_LIBS onnxruntime)
    #    list(APPEND PROJECT_SOURCES ../ONNXEngine.cpp)
    #else ()
    #    message("onnxruntime library was NOT found. NOT linking ONNX runtime!")
    #endif ()

    #if (TRT_LIB AND USE_TENSORRT_RUNTIME)
    #    message("TensorRT library was found: ${TRT_LIB}")
    #    list(APPEND PROJECT_TESTS TRTEngineTest.cpp)
    #    list(APPEND PROJECT_LIBS cudart nvinfer nvonnxparser)
    #    list(APPEND PROJECT_SOURCES ../TRTEngine.cpp)
    #    list(APPEND PROJECT_SOURCES ../dataProcessing/TRTEngineMIMO.cpp)
    #else ()
    #    message("TensorRT library was NOT found. NOT linking TRT runtime!")
    #endif ()

ENDIF ()

#configure_file (../include/AnomalyDetectionConfig.h.in ../../include/AnomalyDetectionConfig.h @ONLY)

# 'Google_Tests_run' is the target name
add_executable(Google_Tests_run ${PROJECT_TESTS} ${PROJECT_SOURCES})

#set_property(TARGET Google_Tests_run PROPERTY
#        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(Google_Tests_run ${PROJECT_LIBS}) #stdc++fs

#target_compile_options(Google_Tests_run PRIVATE "-masm=intel")