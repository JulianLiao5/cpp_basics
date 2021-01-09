cmake_minimum_required(VERSION 3.0)
project(testA_sdk)

set(CMAKE_CXX_STANDARD 14)

add_library(testA_sdk INTERFACE)
target_include_directories(testA_sdk INTERFACE include include)

target_link_libraries(testA_sdk INTERFACE ${CMAKE_CURRENT_LIST_DIR}/lib/libtestA.a)

#add_executable(testA_sample2
#        src/testA_sample2.cpp
#        )
#
#target_link_libraries(testA_sample2 PUBLIC
#        testA_sdk
#        )
