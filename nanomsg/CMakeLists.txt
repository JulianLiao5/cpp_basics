CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)
PROJECT(test_reqrep)

include_directories(include)

add_executable(test_reqrep_REQUEST test_reqrep_REQUEST.cpp)
target_link_libraries(test_reqrep_REQUEST ${CMAKE_SOURCE_DIR}/lib/libpi_msg_utils.a nanomsg pthread)

add_executable(test_reqrep_REPLY test_reqrep_REPLY.cpp)
target_link_libraries(test_reqrep_REPLY ${CMAKE_SOURCE_DIR}/lib/libpi_msg_utils.a nanomsg pthread)
