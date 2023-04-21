cmake_minimum_required(VERSION 3.16)

# Toolchain

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/taurusToolchain.cmake)

# Project

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
    -T${CMAKE_SOURCE_DIR}/common/mbl.lds
    -lc -lgcc -lm
    -march=rv32im -mabi=ilp32 -mcmodel=medany
    -Wl,--gc-sections -Wl,-Map=${OUTPUT_NAME}.map
    -nostartfiles -fno-builtin-printf -fno-builtin-puts
    -fdata-sections -ffunction-sections -fno-builtin-memcmp
)

target_link_options(taurus
    PUBLIC
    -T${CMAKE_SOURCE_DIR}/common/mbl.lds
    -lc -lgcc -lm
    -march=rv32im -mabi=ilp32 -mcmodel=medany
    -Wl,--gc-sections -Wl,-Map=${OUTPUT_NAME}.map
    -nostartfiles -fno-builtin-printf -fno-builtin-puts
    -fdata-sections -ffunction-sections -fno-builtin-memcmp
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
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/taurusToolchain.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/taurusConfigVersion.cmake
    DESTINATION
    "${INSTALL_PREFIX}/lib/cmake/taurus"
)
