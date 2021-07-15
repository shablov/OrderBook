cmake_minimum_required(VERSION 3.17)
project(orderbook LANGUAGES CXX)

file(GLOB_RECURSE SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
#target_link_libraries(${PROJECT_NAME} PRIVATE )
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)
set_common_project_properties(${PROJECT_NAME})

find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

file(GLOB_RECURSE SOURCE_TEST_FILES "${PROJECT_SOURCE_DIR}/test/*.cpp")

set(PROJECT_TEST_NAME ${PROJECT_NAME}_test)
add_executable(${PROJECT_TEST_NAME} ${SOURCE_TEST_FILES})
target_link_libraries(${PROJECT_TEST_NAME} PRIVATE ${PROJECT_NAME} GTest::GTest)
set_common_project_properties(${PROJECT_TEST_NAME})

file(GLOB_RECURSE SOURCE_TEST_FILES "${PROJECT_SOURCE_DIR}/benchmark/*.cpp")

set(PROJECT_BENCHMARK_NAME ${PROJECT_NAME}_benchmark)
add_executable(${PROJECT_BENCHMARK_NAME} ${SOURCE_TEST_FILES})
target_link_libraries(${PROJECT_BENCHMARK_NAME} PRIVATE ${PROJECT_NAME} benchmark::benchmark)
set_common_project_properties(${PROJECT_BENCHMARK_NAME})
