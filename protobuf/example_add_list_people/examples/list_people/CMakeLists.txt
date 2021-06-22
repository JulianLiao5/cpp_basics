cmake_minimum_required(VERSION 3.5)
project(list_people VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O3 -std=c++11")

# Find required protobuf package
find_package(Protobuf REQUIRED)

# Print version messages
if (Protobuf_FOUND)
    message(STATUS "Using Protocol Buffers ${Protobuf_VERSION}")
endif()

add_executable(${PROJECT_NAME} ./list_people.cc)
target_link_libraries(${PROJECT_NAME} PUBLIC addressbook ${Protobuf_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${Protobuf_INCLUDE_DIRS})
