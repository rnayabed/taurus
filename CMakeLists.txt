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

cmake_minimum_required(VERSION 3.16)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/taurusToolchain.cmake)

set(VERSION 1.0)

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
    common/crt.S
    common/rawfloat.c
    common/stdlib.c

    ${HEADERS}

    drivers/uart.c
    drivers/debug_uart.c
    drivers/gpio.c
    drivers/i2c.c
    drivers/qspi.c
    drivers/spi.c
    drivers/timer.c
    drivers/interrupt.c
    drivers/pwmc.c
)

set_target_properties(taurus PROPERTIES
    PUBLIC_HEADER "${HEADERS}"
)

target_include_directories(taurus PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_options(taurus PUBLIC
    -T${CMAKE_SOURCE_DIR}/common/mbl.lds
    -lc -lgcc -lm
    -march=rv32im -mabi=ilp32 -mcmodel=medany
    -Wl,--gc-sections -Wl,-Map=${OUTPUT_NAME}.map
    -nostartfiles -fno-builtin-printf -fno-builtin-puts
    -fdata-sections -ffunction-sections -fno-builtin-memcmp
)

target_link_options(taurus PUBLIC
    -T${CMAKE_SOURCE_DIR}/common/mbl.lds
    -lc -lgcc -lm
    -march=rv32im -mabi=ilp32 -mcmodel=medany
    -Wl,--gc-sections -Wl,-Map=${OUTPUT_NAME}.map
    -nostartfiles -fno-builtin-printf -fno-builtin-puts
    -fdata-sections -ffunction-sections -fno-builtin-memcmp
)

install(TARGETS taurus
    EXPORT taurus-export
    PUBLIC_HEADER DESTINATION include/taurus
    ARCHIVE DESTINATION lib
)

install(EXPORT taurus-export
    FILE taurusTargets.cmake
    NAMESPACE taurus::
    DESTINATION lib/cmake/taurus
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/taurusConfigVersion.cmake
    VERSION ${VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/taurusConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/taurusConfig.cmake
    @ONLY
)

install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/taurusToolchain.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/taurusConfigVersion.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/taurusConfig.cmake
    DESTINATION lib/cmake/taurus
)

if(CMAKE_HOST_UNIX)
    add_custom_target(uninstall COMMAND
        xargs rm -rfv < install_manifest.txt &&
        rm -rfv ${CMAKE_INSTALL_PREFIX}/lib/cmake/taurus &&
        rm -rfv ${CMAKE_INSTALL_PREFIX}/include/taurus
    )
endif()

