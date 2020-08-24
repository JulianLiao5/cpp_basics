cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O3 -std=c++11")

project(cpp_basics)
# add_executable(test_condition_variable test_condition_variable.cpp)
# target_link_libraries(test_condition_variable pthread)

add_executable(test_high_resolution_clock test_high_resolution_clock.cpp)

set(SRC_DATA_STRUCT data_struct/test_data_struct.cc)
add_executable(test_data_struct ${SRC_DATA_STRUCT})

add_executable(test_vector data_struct/test_vector.cc)
add_executable(test_vector_insert data_struct/test_vector_insert.cc)

set(SRC_MATH math/test_math.cc)
add_executable(test_math ${SRC_MATH})

add_executable(test_math_sincos math/test_math_sincos.cc)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(SRC_OPENCV opencv/test_opencv.cpp)
add_executable(test_opencv ${SRC_OPENCV})
target_link_libraries(test_opencv ${OpenCV_LIBS})

add_executable(calc_euclidean_dist opencv/calc_euclidean_dist.cpp)
target_link_libraries(calc_euclidean_dist ${OpenCV_LIBS})
add_executable(test_try_lock mutex/test_try_lock.cpp)
target_link_libraries(test_try_lock pthread)

add_executable(test_thread_id mutex/test_thread_id.cpp)
target_link_libraries(test_thread_id pthread)

add_executable(test_call_once mutex/test_call_once.cpp)
target_link_libraries(test_call_once pthread)

# [BEGIN - file_stream]
add_executable(test_fstream_mode file_stream/test_fstream_mode.cpp)
target_link_libraries(test_fstream_mode pthread)
# [END - file_stream]

# [BEGIN - functional]
add_executable(test_bind functional/test_function_bind.cpp)
# [END - functional]
