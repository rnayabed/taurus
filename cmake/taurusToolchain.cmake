#[[
Copyright (C) 2023 Debayan Sutradhar (rnayabed) (debayansutradhar3@gmail.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
]]

# Check Target
if(TAURUS_TARGET STREQUAL "THEJAS32")
    set(RISCV_ARCH "rv32im")
    set(RISCV_ABI "ilp32")
elseif(TAURUS_TARGET STREQUAL "THEJAS64")
    set(RISCV_ARCH "rv64ima")
    set(RISCV_ABI "lp64")
elseif(TAURUS_TARGET STREQUAL "CDACFPGA")
    set(RISCV_ARCH "rv64imafd")
    set(RISCV_ABI "lp64d")
else()
    message(FATAL_ERROR "Invalid TAURUS_TARGET specified")
endif()



# Check toolchain Prefix
if(NOT TAURUS_TOOLCHAIN_PREFIX)
    message(FATAL_ERROR "TAURUS_TOOLCHAIN_PREFIX required")
endif()



# Configure toolchain path if provided
if (TAURUS_TOOLCHAIN_PATH AND NOT TAURUS_TOOLCHAIN_PATH STREQUAL "")
    set(TAURUS_TOOLCHAIN_FULL_PATH ${TAURUS_TOOLCHAIN_PATH}/bin/${TAURUS_TOOLCHAIN_PREFIX})
else()
    set(TAURUS_TOOLCHAIN_FULL_PATH ${TAURUS_TOOLCHAIN_PREFIX})
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
