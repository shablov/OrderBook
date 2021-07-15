cmake_minimum_required(VERSION 3.17)
if (APPLE)
    set(CMAKE_C_COMPILER "/usr/bin/clang" CACHE STRING "clang compiler" FORCE)
    set(CMAKE_CXX_COMPILER "/usr/bin/clang++" CACHE STRING "clang++ compiler" FORCE)
endif(APPLE)

project(order_book LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-pedantic)
add_compile_options(-Wconversion)
add_compile_options(-Werror)
add_compile_options(-Wfatal-errors)
add_compile_options(-Wno-exceptions)

include(${CMAKE_SOURCE_DIR}/conan.cmake)
set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR};${CMAKE_MODULE_PATH}")

conan_cmake_run(CONANFILE conanfile.txt
        BUILD_TYPE "Release"
        BUILD missing
        )

find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

add_executable(order_book src/main.cpp src/OrderBook.cpp)