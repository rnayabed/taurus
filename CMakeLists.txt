cmake_minimum_required(VERSION 3.16)

set(VERSION 1.0)

set(RISCV_TOOLCHAIN_PATH "/opt/risc v/bin")
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

set(CMAKE_C_FLAGS                   " -Wl,--gc-sections, -Wl,-Map,${OUTPUT_NAME}.map -nostartfiles -nostdlib -march=${RISCV_ARCH} -mabi=${RISCV_ABI} -mcmodel=medany -fno-builtin-printf -fno-builtin-puts -fdata-sections -ffunction-sections -fno-builtin-memcmp --specs=nano.specs -specs=nosys.specs" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS                 "${CMAKE_C_FLAGS}" CACHE INTERNAL "")

#set(CMAKE_C_FLAGS_DEBUG             "-Og -g" CACHE INTERNAL "")
#set(CMAKE_C_FLAGS_RELEASE           "-O3 -DNDEBUG" CACHE INTERNAL "")
#set(CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_C_FLAGS_DEBUG}" CACHE INTERNAL "")
#set(CMAKE_CXX_FLAGS_RELEASE         "${CMAKE_C_FLAGS_RELEASE}" CACHE INTERNAL "")

#set(CMAKE_STATIC_LINKER_FLAGS       "-Wl,--gc-sections, -Wl,-Map,${OUTPUT_NAME}.map -nostartfiles -nostdlib -march=${RISCV_ARCH} -mabi=${RISCV_ABI} -mcmodel=medany")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

project(taurus 
    VERSION ${VERSION}
    LANGUAGES C ASM
    DESCRIPTION "SDK For CDAC Vega Processors"
)

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
    include/stdlib.h
)

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
LINK_DEPENDS "${CMAKE_SOURCE_DIR}/common/mbl.lds"
)

target_include_directories(taurus PRIVATE include drivers)

#include(GNUInstallDirs)
set(CMAKE_INSTALL_LIBDIR /home/rnayabed/code/rnayabed/taurus-sdk/test_install/lib)
set(CMAKE_INSTALL_INCLUDEDIR /home/rnayabed/code/rnayabed/taurus-sdk/test_install/public_header)

install(TARGETS taurus 
    EXPORT taurus DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
