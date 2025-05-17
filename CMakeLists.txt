# ----------------------------------------------------------------------------
# Copyright (c) 2025 by Systream. All rights reserved.
#
# File Description: CMake Project File
# ----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.14)

project(
  "stabor-evse-plc"
  VERSION "0.0.1"
  DESCRIPTION "EVSE Application"
  LANGUAGES CXX C)

add_subdirectory(evse_impl)
add_subdirectory(evse)
