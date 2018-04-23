cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O3 -std=c++11")

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

add_executable(converter_pi_3d_2_2d src/pi_vio_3d_2_2d.cpp)
target_link_libraries(converter_pi_3d_2_2d)
