#!/bin/bash

TAURUS_SDK=`dirname -- $(readlink -f "${BASH_SOURCE}")`
VERSION=1.0
CREATE_MINICOM_CONFIG=1
MINICOM_CONFIG=/etc/minirc.aries
CREATE_GLOBAL_MAKEFILE=1
GLOBAL_MAKEFILE=~/.config/vega-tools/settings.mk
LICENSE_URL=https://github.com/rnayabed/taurus-sdk/blob/master/LICENSE

VALID_TARGETS=("THEJAS32" "THEJAS64" "CDACFPGA")

usage() {
  printf "
Usage:  [-t | --target]
        [-tp | --toolchain-prefix] [-ta | --toolchain-path]
        [-mp | --global-makefile-path]
        [-sm | --skip-global-makefile]
        [-nm | --no-minicom] [-h | --help]

Option Summary:
    -t | --target                       Required. Set the target to build SDK for. 
                                        Valid targets are:
                                        %s
    
    -tp | --toolchain-prefix            Required. RISC-V GNU Compiler Toolchain prefix.
                                        Example: 'riscv64-unknown-elf'

    -ta | --toolchain-path              Optional. Specify the absolute path of toolchain
                                        if it is not present in PATH.

    -gp | --global-makefile-path        Optional. Global makefile path with 
                                        TAURUS_SDK, TAURUS_COMPILER_PREFIX, TAURUS_TARGET
                                        and TAURUS_TOOLCHAIN_PATH properties. You may skip
                                        this and use environment variables instead.
    
    -sm | --skip-global-makefile        Optional. Skips the creation of global Makefile.

    -nm | --no-minicom                  Optional. Do not create minicom configuration file. 
                                        Configuration is created if not specified.

    -h  --help                          Print this message.
" "${VALID_TARGETS[*]}"
}

parse_params() {
    while :; do
        case "${1-}" in
        -t | --target)
            TAURUS_TARGET="${2-}"
            shift
            ;;
        -tp | --toolchain-prefix)
            TAURUS_COMPILER_PREFIX="${2-}"
            shift
            ;;
        -ta | --toolchain-path)
            TAURUS_TOOLCHAIN_PATH="${2-}"
            shift
            ;;
        -gp | --global-makefile-path)
            GLOBAL_MAKEFILE="${2-}"
            shift
            ;;
        -sm | --skip-global-makefile)
            CREATE_GLOBAL_MAKEFILE=0
            ;;
        -nm | --no-minicom)
            CREATE_MINICOM_CONFIG=0
            ;;
        *) 
            if [ ! -z "${1-}" -a "${1-}" != " " ]; then
                printf "Invalid option %s\n" "${1-}"
                usage 
                exit 1
            else
                break
            fi
            ;;
        esac
        shift
    done
} 


parse_params "$@"

if [[ -z ${TAURUS_COMPILER_PREFIX+x} ]]; then
    printf "Toolchain prefix not provided.\n"
    ERROR=1
fi

if [[ -z ${TAURUS_TARGET+x} ]]; then
    printf "Target not provided.\n"
    ERROR=1
elif [[  ! " ${VALID_TARGETS[*]} " =~ " $TAURUS_TARGET " ]]; then
    printf "Invalid target provided
Valid targets are %s.\n" "${VALID_TARGETS[*]}"
    ERROR=1
fi

if [[ $ERROR -eq 1 ]]; then
    usage
    exit 1
fi


printf "

G#&G:               :G&#G
  :#@Y             Y@#:  
    Y@#.         .#@Y    
     ^&&?.     .?&&^     
       J@@&BGB&@@J       
     ~&@5~.. ..^5@&~     
    Y@G           G@Y    
   .@&             &@.   
   :@&             &@.   
    5@P           P@5    
     !&&5^.   .^5&&!     
       ~P&&&&&&&P~       

       Taurus SDK 
          v%s

Copyright (C) 2023 Debayan Sutradhar

This program comes with ABSOLUTELY NO WARRANTY.
This is free software, and you are welcome to redistribute it
under certain conditions.
    
Full license can be found in the 'LICENSE' file provided with the SDK.
The license can also be viewed by visiting %s

" "$VERSION" "$LICENSE_URL"


if [[ ${CREATE_GLOBAL_MAKEFILE} -eq 1 ]]; then
printf "\nSetting up Global Make file\n"
printf "# Tarus SDK - Global Configuration
TAURUS_SDK=%s
TAURUS_COMPILER_PREFIX=%s
TAURUS_TARGET=%s
" "$TAURUS_SDK" "$TAURUS_COMPILER_PREFIX" "$TAURUS_TARGET" | tee $GLOBAL_MAKEFILE > /dev/null 

if [[ ! -z ${TAURUS_TOOLCHAIN_PATH+x} ]]; then
    printf "TAURUS_TOOLCHAIN_PATH=%s
" "$TAURUS_TOOLCHAIN_PATH" | tee -a $GLOBAL_MAKEFILE > /dev/null 
fi
fi



printf "\nCompiling Taurus SDK for %s\n" "$TAURUS_TARGET"
make

if [[ $? -ne 0 ]]; then
    printf "Failed to compile Taurus SDK.\n"
    exit 1
fi



if [[ "$CREATE_MINICOM_CONFIG" -eq 1 ]]; then
    printf "\nCreating Minicom configuration file\n"
    printf "# Taurus SDK - Minicom Configuration for CDAC Vega
pu port             /dev/ttyUSB0
pu baudrate         115200
pu bits             8
pu parity           N
pu stopbits         1
pu updir            %s/bin
pu rtscts           No 
" "$TAURUS_SDK" | sudo tee ${MINICOM_CONFIG} > /dev/null

if [[ $? -ne 0 ]]; then
    printf "Failed to create Minicom configuration file\n"
    exit 1
fi

fi


printf "\n=====================================================================
Taurus SDK %s for %s is now ready to use.
" "$VERSION" "$TAURUS_TARGET"

if [[ ${CREATE_GLOBAL_MAKEFILE} -eq 1 ]]; then
    printf "\nA global Makefile has also been created at 
%s
You can include this in your project Makefile to include important
environment variables.\n" "$GLOBAL_MAKEFILE"
fi


if [[ ${CREATE_MINICOM_CONFIG} -eq 1 ]]; then
    printf "\nMinicom configuration has also been created at
%s
You can access it by running 'sudo minicom aries'.
You may also add yourself in the "dialout" user group to use
minicom without root permissions.\n" "$MINICOM_CONFIG"
fi

printf "
Enjoy!
=====================================================================\n"