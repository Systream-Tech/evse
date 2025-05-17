# ----------------------------------------------------------------------------
# Copyright (c) 2025 by Systream. All rights reserved.
#
# File Description: CMake Module File
# ----------------------------------------------------------------------------
set(TARGET_NAME evse_impl)

add_library(${TARGET_NAME} INTERFACE)

target_sources(
  ${TARGET_NAME}
  INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/evse.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/iso15118_2/sdp_server/sdp_server.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/iso15118_3/slac/evse_fsm.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/iso15118_3/slac/slac_io.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/evse.cpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/iso15118_2/sdp_server/sdp_server.cpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/iso15118_3/slac/evse_fsm.cpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/iso15118_3/slac/slac_io.cpp>"
)

target_include_directories(
  ${TARGET_NAME}
  INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>")

find_package(everest-cmake 0.5 PATHS ../everest-cmake)

if(NOT everest-cmake_FOUND)
  message(STATUS "Retrieving everest-cmake using FetchContent")
  include(FetchContent)
  FetchContent_Declare(
    everest-cmake
    GIT_REPOSITORY https://github.com/EVerest/everest-cmake.git
    GIT_TAG v0.5.0)
  FetchContent_MakeAvailable(everest-cmake)
  set(everest-cmake_DIR "${everest-cmake_SOURCE_DIR}")
  include("${everest-cmake_SOURCE_DIR}/everest-cmake-config.cmake")
endif()

# dependencies
if(NOT DISABLE_EDM)
  evc_setup_edm()
  # In EDM mode, we can't install exports (because the dependencies usually do
  # not install their exports)
  set(ISO15118_INSTALL OFF)
else()
  find_package(cbv2g REQUIRED)
endif()

target_link_libraries(${TARGET_NAME} INTERFACE iso15118)
# target_link_libraries(${TARGET_NAME} INTERFACE fmt)
target_link_libraries(${TARGET_NAME} INTERFACE slac)
