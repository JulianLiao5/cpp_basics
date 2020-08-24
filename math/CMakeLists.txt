cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O3 -std=c++11")

project(math)

set(SRC_MATH test_math.cc)
add_executable(test_math ${SRC_MATH})

add_executable(test_math_sincos test_math_sincos.cc)
