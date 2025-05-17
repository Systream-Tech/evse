# ----------------------------------------------------------------------------
# Copyright (c) 2025 by Systream. All rights reserved.
#
# File Description: CMake Toolchain File
# ----------------------------------------------------------------------------

# Target processor
set(CMAKE_SYSTEM_PROCESSOR
    arm
    CACHE STRING "default processor if not configured")

# Add compiler flags applied to all targets add_compile_options( -fPIC -Wall
# -Wextra -Werror -Wunreachable-code -Wuninitialized -Wshadow -Wdouble-promotion
# -Wformat=2 -Wformat-truncation -Wundef -fno-common -fstack-usage)

# Ignore intentional 'shadow' warning thrown from third party libraries
add_compile_options(-Wno-error=shadow)
# Allow float to double conversion only for third party libraries
add_compile_options(-Wno-error=double-promotion)
# TODO: Enable -pedantic-errors and -pedantic flag only for production software

# Path to installed toolchain TODO => Robust solution to find toolchain
set(CMAKE_SYSROOT
    $ENV{HOME}/toolchains/stm32_toolchain_scarthgap/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi
)
if(NOT EXISTS ${CMAKE_SYSROOT})
  message(FATAL_ERROR "Directory does NOT exist: ${CMAKE_SYSROOT}")
endif()

# TODO=> Recursively search the compiler in installed toolchain path prefix that
# can be applied to compiler's utilities
set(CMAKE_TOOLCHAIN_PREFIX
    arm-ostl-linux-gnueabi-
    CACHE STRING "Configurable toolchain prefix")
# C Compiler
set(CMAKE_C_COMPILER ${CMAKE_TOOLCHAIN_PREFIX}gcc)
# C++ Complier
set(CMAKE_CXX_COMPILER ${CMAKE_TOOLCHAIN_PREFIX}g++)

# Necesarry compiler flags
set(CMAKE_C_FLAGS
    "-march=armv7ve -mthumb -mfpu=neon-vfpv4 -mfloat-abi=hard -mcpu=cortex-a7 --sysroot=${CMAKE_SYSROOT} "
    CACHE STRING "CFLAGS")
set(CMAKE_CXX_FLAGS
    "-march=armv7ve -mthumb -mfpu=neon-vfpv4 -mfloat-abi=hard -mcpu=cortex-a7 --sysroot=${CMAKE_SYSROOT} -fpermissive"
    CACHE STRING "CXXFLAGS")
set(CMAKE_C_FLAGS_RELEASE
    "-march=armv7ve -mthumb -mfpu=neon-vfpv4 -mfloat-abi=hard -mcpu=cortex-a7 --sysroot=${CMAKE_SYSROOT} -DNDEBUG"
    CACHE STRING "CFLAGS for release")
set(CMAKE_CXX_FLAGS_RELEASE
    "-march=armv7ve -mthumb -mfpu=neon-vfpv4 -mfloat-abi=hard -mcpu=cortex-a7 --sysroot=${CMAKE_SYSROOT}  -DNDEBUG"
    CACHE STRING "CXXFLAGS for release")

# No Linker flags required
set(CMAKE_C_LINK_FLAGS
    ""
    CACHE STRING "LDFLAGS")
set(CMAKE_CXX_LINK_FLAGS
    ""
    CACHE STRING "LDFLAGS")
