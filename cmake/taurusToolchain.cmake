#[[
SPDX-License-Identifier: MIT
CMake project toolchain configurator
Author: Debayan Sutradhar
]]



# Check Target Board
if (TAURUS_TARGET_BOARD STREQUAL "ARIES_V2")
    add_compile_definitions(TARGET_BOARD_ARIES_V2)
    set(TAURUS_TARGET_SOC THEJAS32)
elseif (TAURUS_TARGET_BOARD STREQUAL "ARIES_V3")
    add_compile_definitions(TARGET_BOARD_ARIES_V3)
    set(TAURUS_TARGET_SOC THEJAS32)
elseif (TAURUS_TARGET_BOARD STREQUAL "ARIES_MICRO_V1")
    add_compile_definitions(TARGET_BOARD_ARIES_MICRO_V1)
    set(TAURUS_TARGET_SOC THEJAS32)
elseif (TAURUS_TARGET_BOARD STREQUAL "ARIES_IOT_V1")
    add_compile_definitions(TARGET_BOARD_ARIES_IOT_V1)
    set(TAURUS_TARGET_SOC THEJAS32)
elseif(TAURUS_TARGET_BOARD)
    message(FATAL_ERROR "Invalid TAURUS_TARGET_BOARD specified")
endif()



# Check Target SoC
if(TAURUS_TARGET_SOC STREQUAL "THEJAS32")
    add_compile_definitions(TARGET_SOC_THEJAS32)
    set(RISCV_ARCH rv32im)
    set(RISCV_ABI ilp32)
elseif(TAURUS_TARGET_SOC STREQUAL "THEJAS64")
    add_compile_definitions(TARGET_SOC_THEJAS64)
    set(RISCV_ARCH rv64ima)
    set(RISCV_ABI lp64)
elseif(TAURUS_TARGET_SOC STREQUAL "CDACFPGA")
    add_compile_definitions(TARGET_SOC_CDACFPGA)
    set(RISCV_ARCH rv64imafd)
    set(RISCV_ABI lp64d)
else()
    message(FATAL_ERROR "Invalid TAURUS_TARGET_SOC specified")
endif()

set(RISCV_CMODEL medany)



# Check toolchain triplet
if(NOT TAURUS_TOOLCHAIN_TRIPLET)
    message(FATAL_ERROR "TAURUS_TOOLCHAIN_TRIPLET required")
endif()



# Configure toolchain path if provided
if (TAURUS_TOOLCHAIN_PATH AND NOT TAURUS_TOOLCHAIN_PATH STREQUAL "")
    set(TAURUS_TOOLCHAIN_FULL_PATH ${TAURUS_TOOLCHAIN_PATH}/bin/${TAURUS_TOOLCHAIN_TRIPLET})
else()
    set(TAURUS_TOOLCHAIN_FULL_PATH ${TAURUS_TOOLCHAIN_TRIPLET})
endif()



# Configure executable suffix for Windows
if (CMAKE_HOST_WIN32)
    set(CMAKE_EXECUTABLE_SUFFIX .exe)
endif()



# Configure toolchain
set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          riscv)
set(CMAKE_TRY_COMPILE_TARGET_TYPE   STATIC_LIBRARY)

set(CMAKE_AR                        ${TAURUS_TOOLCHAIN_FULL_PATH}-ar${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_ASM_COMPILER              ${TAURUS_TOOLCHAIN_FULL_PATH}-gcc${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_C_COMPILER                ${TAURUS_TOOLCHAIN_FULL_PATH}-gcc${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_CXX_COMPILER              ${TAURUS_TOOLCHAIN_FULL_PATH}-g++${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_LINKER                    ${TAURUS_TOOLCHAIN_FULL_PATH}-ld${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_OBJCOPY                   ${TAURUS_TOOLCHAIN_FULL_PATH}-objcopy${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_RANLIB                    ${TAURUS_TOOLCHAIN_FULL_PATH}-ranlib${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_SIZE                      ${TAURUS_TOOLCHAIN_FULL_PATH}-size${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_STRIP                     ${TAURUS_TOOLCHAIN_FULL_PATH}-strip${CMAKE_EXECUTABLE_SUFFIX})



# Search includes and packages in toolchain
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
