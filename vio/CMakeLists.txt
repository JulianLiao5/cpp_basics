cmake_minimum_required(VERSION 3.0)
project(msckf_vio)

add_compile_options(-std=c++14)
if(NOT CMAKE_BUILD_TYPE) 
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

option(BUILD_ON_NONTEGRA "Build on platform other than tegra tx1" ON)

# Modify cmake module path if new .cmake files are required
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L/usr/lib/aarch64-linux-gnu/tegra")

## System dependencies are found with CMake's conventions
find_package(Boost REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(SuiteSparse REQUIRED)

message("opencv: ${OpenCV_INCLUDE_DIRS}")

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

    message("openmp found: ${OpenMP_CXX_FLAGS}")
endif()

add_subdirectory(../df_adaptor df_adaptor)

###########
## Build ##
###########

if("${CMAKE_SYSTEM_PROCESSOR}" MATCHES "^x86.*")
  set(PIMSG_LIB_PATH 
  ${CMAKE_SOURCE_DIR}/../third_party/msg_utils/lib/libpi_msg_utils.a
)

  if(APPLE)
    set(YAML_LIB_PATH 
      ${CMAKE_SOURCE_DIR}/../third_party/yaml-cpp/lib/libyaml-cpp_osx.a
    )
  else()
    set(YAML_LIB_PATH 
      ${CMAKE_SOURCE_DIR}/../third_party/yaml-cpp/lib/libyaml-cpp_ubuntu.a
    )
  endif()
else()
    set(PIMSG_LIB_PATH 
      ${CMAKE_SOURCE_DIR}/../third_party/msg_utils/lib/libpi_msg_utils_tegra.a
    )

    set(YAML_LIB_PATH 
      ${CMAKE_SOURCE_DIR}/../third_party/yaml-cpp/lib/libyaml-cpp_tegra.a
    )  
    add_definitions(-DBUILD_ON_JETSON)
    link_directories(/usr/lib/aarch64-linux-gnu/tegra)
endif()

message(STATUS "YAML_LIB_PATH: " ${YAML_LIB_PATH})

add_library(yaml-cpp STATIC IMPORTED)

SET_TARGET_PROPERTIES(yaml-cpp 
              PROPERTIES IMPORTED_LOCATION ${YAML_LIB_PATH}
              )

add_library(msg_utils STATIC IMPORTED)
SET_TARGET_PROPERTIES(msg_utils 
              PROPERTIES IMPORTED_LOCATION ${PIMSG_LIB_PATH}
              )

include_directories(
  include
  ${EIGEN3_INCLUDE_DIR}
  ${Boost_INCLUDE_DIR}
  ${OpenCV_INCLUDE_DIRS}
  ${SUITESPARSE_INCLUDE_DIRS}
  ../third_party/yaml-cpp/include
  ../third_party/msg_utils/include
)

# Msckf Vio
add_executable(msckf_vio
  src/msckf_vio.cpp
  src/image_processor.cpp
  src/utils.cpp
  main.cpp
)

link_directories(/usr/local/lib)

target_link_libraries(msckf_vio
  df_adaptor
  yaml-cpp
  msg_utils
  nanomsg
  pthread
  ${SUITESPARSE_LIBRARIES}
  ${OpenCV_LIBRARIES}
)

#add_executable("vio_viz" viz_vio.cpp)
#target_link_libraries("vio_viz" msg_utils nanomsg ${OpenCV_LIBRARIES})

FILE(COPY ${CMAKE_SOURCE_DIR}/config/global.yaml DESTINATION "${CMAKE_BINARY_DIR}/")

