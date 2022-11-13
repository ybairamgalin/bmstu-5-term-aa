cmake_minimum_required(VERSION 3.23)
project(aa_labs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0")
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "-Werror")

add_subdirectory("lib/googletest")
include_directories(
        ${gtest_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR})

add_executable(lab_02_run_tests
        lab_02/src/tests/test_matrix_multiplication.cpp
        lab_02/src/algorithms/matrix_multiplication.hpp
        lab_02/src/models/matrix.hpp)
target_include_directories(lab_02_run_tests
        PRIVATE lab_02/src)
target_link_libraries(lab_02_run_tests
        gtest
        gtest_main)

add_executable(lab_03_run_tests
        lab_03/src/tests/test_sort.cpp
        lab_03/src/algorithms/sort.hpp)
target_include_directories(lab_03_run_tests
        PRIVATE lab_03/src)
target_link_libraries(lab_03_run_tests
        gtest
        gtest_main)

add_executable(lab_03_measure_execution_time
        lab_03/src/algorithms/sort.hpp
        lab_03/src/utils/execution_time.hpp
        lab_03/src/time_measurement/sort_time_execution.cpp)
target_include_directories(lab_03_measure_execution_time
        PRIVATE lab_03/src)
