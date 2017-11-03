cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O3 -std=c++11")

add_executable(test_condition_variable test_condition_variable.cpp)
target_link_libraries(test_condition_variable pthread)

add_executable(test_high_resolution_clock test_high_resolution_clock.cpp)

set(SRC_DATA_STRUCT data_struct/test_data_struct.cc)
add_executable(test_data_struct ${SRC_DATA_STRUCT})

set(SRC_MATH math/test_math.cc)
add_executable(test_math ${SRC_MATH})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(SRC_OPENCV opencv/test_opencv.cpp)
add_executable(test_opencv ${SRC_OPENCV})
target_link_libraries(test_opencv ${OpenCV_LIBS})
