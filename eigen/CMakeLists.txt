cmake_minimum_required(VERSION 3.5)

project(eigen_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O3 -std=c++11")

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

add_executable(test_eigen_matrix src/test_eigen_matrix.cpp)
target_link_libraries(test_eigen_matrix)

add_executable(eigen_geometry_1 src/eigen_geometry_1.cpp)