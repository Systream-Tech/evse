# ----------------------------------------------------------------------------
# Copyright (c) 2025 by Systream. All rights reserved.
#
# File Description: CMake Module
# ----------------------------------------------------------------------------
set(TARGET_NAME evse_exe)

add_executable(${TARGET_NAME})
target_sources(${TARGET_NAME}
               PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

target_link_libraries(${TARGET_NAME} PRIVATE evse_impl)

target_compile_features(${TARGET_NAME} PUBLIC cxx_std_17)

set(EVSE_APP_INSTALL_BIN_DIR "install/${TARGET_NAME}/bin")

install(TARGETS ${TARGET_NAME}
        RUNTIME DESTINATION "${EVSE_APP_INSTALL_BIN_DIR}")
