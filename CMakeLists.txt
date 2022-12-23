cmake_minimum_required(VERSION 3.23)
project(aa_labs)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(
        -Wall
        -Werror
        -Wextra
        -Wpedantic
        -O0)

add_subdirectory("lib/googletest")
add_subdirectory("lib/fmt")
add_subdirectory("lib/libpqxx")
include_directories(
        ${gtest_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR})

add_executable(lab_01_run_tests
        lab_01/src/tests/test_levenshtein_distance.cpp
        lab_01/src/algorithms/levenshtein_distance.hpp
        lab_01/src/algorithms/levenshtein_distance.cpp)
target_include_directories(lab_01_run_tests
        PRIVATE lab_01/src)
target_link_libraries(lab_01_run_tests
        gtest
        gtest_main)

add_executable(lab_01_execution_time
        lab_01/src/efficiency/execution_time.cpp
        lab_01/src/utils/execution_time.hpp
        lab_01/src/algorithms/levenshtein_distance.hpp
        lab_01/src/algorithms/levenshtein_distance.cpp)
target_include_directories(lab_01_execution_time
        PRIVATE lab_01/src)

add_executable(lab_02_run_tests
        lab_02/src/tests/test_matrix_multiplication.cpp
        lab_02/src/utils/execution_time.hpp
        lab_02/src/algorithms/matrix_multiplication.hpp
        lab_02/src/models/matrix.hpp)
target_include_directories(lab_02_run_tests
        PRIVATE lab_02/src)
target_link_libraries(lab_02_run_tests
        gtest
        gtest_main)

add_executable(lab_02_execution_time
        lab_02/src/efficiency/execution_time.cpp
        lab_02/src/algorithms/matrix_multiplication.hpp
        lab_02/src/models/matrix.hpp)
target_include_directories(lab_02_execution_time
        PRIVATE lab_02/src)

add_executable(lab_03_run_tests
        lab_03/src/tests/test_sort.cpp
        lab_03/src/algorithms/sort.hpp)
target_include_directories(lab_03_run_tests
        PRIVATE lab_03/src)
target_link_libraries(lab_03_run_tests
        gtest
        gtest_main)

add_executable(lab_03_execution_time
        lab_03/src/algorithms/sort.hpp
        lab_03/src/utils/execution_time.hpp
        lab_03/src/time_measurement/sort_time_execution.cpp)
target_include_directories(lab_03_execution_time
        PRIVATE lab_03/src)

add_executable(lab_04_server
        lab_04/src/main.cpp
        lab_04/src/components.hpp
        lab_04/src/components.cpp
        lab_04/src/views/multiply_matrix/handler.hpp
        lab_04/src/views/multiply_matrix/handler.cpp
        lab_04/src/views/handler.cpp
        lab_04/src/views/handler.hpp
        lab_04/src/models/request.hpp
        lab_04/src/models/response.hpp
        lab_04/src/models/response.cpp
        lab_04/src/models/matrix.hpp
        lab_04/src/utils/to_string.hpp
        lab_04/src/utils/parse.hpp
        lab_04/src/utils/parse.cpp
        lab_04/src/pool/thread_pool.hpp
        lab_04/src/pool/blocking_queue.hpp
        lab_04/src/server/server.cpp
        lab_04/src/server/server.hpp)
target_include_directories(lab_04_server
        PRIVATE lab_04/src
        PRIVATE lib/fmt/include/)
target_link_libraries(lab_04_server
        fmt)

add_executable(lab_05_server_conveyor
        lab_05/src/main.cpp
        lab_05/src/components.hpp
        lab_05/src/components.cpp
        lab_05/src/server/server.cpp
        lab_05/src/server/server.hpp
        lab_05/src/pool/blocking_queue.hpp
        lab_05/src/pool/thread_pool.hpp
        lab_05/src/models/response.hpp
        lab_05/src/models/response.cpp
        lab_05/src/models/request.hpp
        lab_05/src/utils/parse.hpp
        lab_05/src/utils/parse.cpp
        lab_05/src/utils/to_string.hpp
        lab_05/src/views/handler.hpp
        lab_05/src/views/handler.cpp)
target_include_directories(lab_05_server_conveyor
        PRIVATE lab_05/src
        PRIVATE lib/fmt/include
        PRIVATE lib/libpqxx/include)
target_link_libraries(lab_05_server_conveyor
        fmt
        pqxx)
