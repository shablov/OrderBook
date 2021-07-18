cmake_minimum_required(VERSION 3.16)
if (APPLE)
    set(CMAKE_C_COMPILER "/usr/bin/clang" CACHE STRING "clang compiler" FORCE)
    set(CMAKE_CXX_COMPILER "/usr/bin/clang++" CACHE STRING "clang++ compiler" FORCE)
endif(APPLE)

project(alberblanc)

include(${CMAKE_SOURCE_DIR}/build_utils.cmake)

include(${CMAKE_SOURCE_DIR}/conan.cmake)
set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR};${CMAKE_MODULE_PATH}")
conan_cmake_run(CONANFILE conanfile.txt
        BUILD_TYPE "Release"
        BUILD missing
        )

add_subdirectory(src/orderbook)