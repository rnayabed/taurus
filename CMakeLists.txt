cmake_minimum_required(VERSION 3.16)

# Toolchain

set(RISCV_TOOLCHAIN_PATH "/opt/riscv/bin")
set(RISCV_TOOLCHAIN_PREFIX riscv64-unknown-elf)
set(RISCV_ARCH "rv32im")
set(RISCV_ABI "ilp32")

#set(CMAKE_SYSTEM_NAME               CDAC Vega)
set(CMAKE_SYSTEM_PROCESSOR          riscv)

set(CMAKE_TRY_COMPILE_TARGET_TYPE   STATIC_LIBRARY)

set(CMAKE_AR                        ${RISCV_TOOLCHAIN_PATH}//${RISCV_TOOLCHAIN_PREFIX}-ar${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_ASM_COMPILER              ${RISCV_TOOLCHAIN_PATH}/${RISCV_TOOLCHAIN_PREFIX}-gcc${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_C_COMPILER                ${RISCV_TOOLCHAIN_PATH}/${RISCV_TOOLCHAIN_PREFIX}-gcc${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_CXX_COMPILER              ${RISCV_TOOLCHAIN_PATH}/${RISCV_TOOLCHAIN_PREFIX}-g++${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_LINKER                    ${RISCV_TOOLCHAIN_PATH}/${RISCV_TOOLCHAIN_PREFIX}-ld${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_OBJCOPY                   ${RISCV_TOOLCHAIN_PATH}/${RISCV_TOOLCHAIN_PREFIX}-objcopy${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_RANLIB                    ${RISCV_TOOLCHAIN_PATH}/${RISCV_TOOLCHAIN_PREFIX}-ranlib${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_SIZE                      ${RISCV_TOOLCHAIN_PATH}/${RISCV_TOOLCHAIN_PREFIX}-size${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_STRIP                     ${RISCV_TOOLCHAIN_PATH}/${RISCV_TOOLCHAIN_PREFIX}-strip${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")

#set(CMAKE_C_FLAGS                   "--specs=nano.specs -lc -lgcc -lm -march=rv32im -mabi=ilp32 -mcmodel=medany -T/home/rnayabed/code/rnayabed/taurus/common/mbl.lds -Wl,--gc-sections -Wl,-Map=${OUTPUT_NAME}.map -nostartfiles -fno-builtin-printf -fno-builtin-puts -fdata-sections -ffunction-sections -fno-builtin-memcmp" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS                 "${CMAKE_C_FLAGS}" CACHE INTERNAL "")

#set(CMAKE_C_FLAGS_DEBUG             "-Og -g" CACHE INTERNAL "")
#set(CMAKE_C_FLAGS_RELEASE           "-O3 -DNDEBUG" CACHE INTERNAL "")
#set(CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_C_FLAGS_DEBUG}" CACHE INTERNAL "")
#set(CMAKE_CXX_FLAGS_RELEASE         "${CMAKE_C_FLAGS_RELEASE}" CACHE INTERNAL "")



set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)


# Project

set(CMAKE_VERBOSE_MAKEFILE ON)

set(VERSION 1)

project(taurus 
    VERSION ${VERSION}
    LANGUAGES C ASM
    DESCRIPTION "SDK For CDAC Vega Processors")

set(HEADERS
    include/gpio.h
    include/i2c.h
    include/qspi.h
    include/m25p80_eeprom.h
    include/config.h
    include/spi.h
    include/timer.h
    include/uart.h
    include/debug_uart.h
    include/adc.h
    include/interrupt.h
    include/pwmc.h
    include/encoding.h
    include/stdlib.h)

add_library(taurus STATIC
    ${HEADERS}

    common/crt.S
    common/rawfloat.c
    common/stdlib.c

    drivers/uart.c
    drivers/debug_uart.c
    drivers/gpio.c
    drivers/i2c.c
    drivers/qspi.c
    drivers/spi.c
    drivers/timer.c
    drivers/interrupt.c
    drivers/pwmc.c

    common/stdlib.c
    common/rawfloat.c
    common/crt.S
)

set_target_properties(taurus PROPERTIES
    #LINK_DEPENDS "${CMAKE_SOURCE_DIR}/common/mbl.lds"
    PUBLIC_HEADER "${HEADERS}"
)

set(INSTALL_PREFIX /home/rnayabed/code/rnayabed/gg/tt)

target_include_directories(taurus
    PUBLIC
    $<INSTALL_INTERFACE:${INSTALL_PREFIX}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_options(taurus
    PUBLIC
    -T/home/rnayabed/code/rnayabed/taurus/common/mbl.lds -lc -lgcc -lm -march=rv32im -mabi=ilp32 -mcmodel=medany -Wl,--gc-sections -Wl,-Map=${OUTPUT_NAME}.map -nostartfiles -fno-builtin-printf -fno-builtin-puts -fdata-sections -ffunction-sections -fno-builtin-memcmp
)

install(TARGETS taurus
    EXPORT taurus-export
    PUBLIC_HEADER DESTINATION ${INSTALL_PREFIX}/include/taurus
    ARCHIVE DESTINATION ${INSTALL_PREFIX}/lib
)

install(EXPORT taurus-export
    FILE taurusTargets.cmake
    NAMESPACE taurus::
    DESTINATION ${INSTALL_PREFIX}/lib/cmake/taurus
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/taurusConfigVersion.cmake
    VERSION 1.0
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/taurusConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/taurusConfigVersion.cmake
    DESTINATION
    "${INSTALL_PREFIX}/lib/cmake/taurus"
)
