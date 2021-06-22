cmake_minimum_required(VERSION 3.5)
project(proto_from_file VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O3 -std=c++11")

find_package(Glog REQUIRED)
include_directories(${GLOG_INCLUDE_DIRS})

# Print version messages
if (Glog_FOUND)
    message(STATUS "Using Glog ${Glog_VERSION}")
endif()

# Find required protobuf package
find_package(Protobuf REQUIRED)

# Print version messages
if (Protobuf_FOUND)
    message(STATUS "Using Protocol Buffers ${Protobuf_VERSION}")
endif()

add_executable(${PROJECT_NAME} ./proto_from_file.cc)
target_link_libraries(${PROJECT_NAME} PUBLIC config_calibration_table ${Protobuf_LIBRARIES} glog)
target_include_directories(${PROJECT_NAME} PUBLIC ${Protobuf_INCLUDE_DIRS})
