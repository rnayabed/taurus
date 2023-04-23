#!/bin/bash

# Copyright (C) 2023 Debayan Sutradhar (rnayabed) (debayansutradhar3@gmail.com)

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# Authors : Debayan Sutradhar (@rnayabed)

VERSION=1.0
TAURUS_SDK=`dirname -- $(readlink -f "${BASH_SOURCE}")`

CREATE_MINICOM_CONFIG=1
MINICOM_CONFIG=/etc/minirc.aries

LICENSE_URL=https://github.com/rnayabed/taurus/blob/master/LICENSE
CHANGES_URL=https://github.com/rnayabed/taurus/blob/master/README.md#comparison-with-official-sdk

BUILD_DIR=build
BUILD_TYPE=Debug
BUILD_SYSTEM="Unix Makefiles"

VALID_TARGETS=("THEJAS32" "THEJAS64" "CDACFPGA")


usage() {
  printf "
Usage:  [-t | --target]
        [-tp | --toolchain-prefix] [-ta | --toolchain-path]
        [-ip | --install-path]
        [-nm | --no-minicom] [-h | --help]

Option Summary:
    -t | --target                       Required. Set the target to build SDK for. 
                                        Valid targets are:
                                        %s
    
    -tp | --toolchain-prefix            Required. RISC-V GNU Compiler Toolchain prefix.
                                        Example: 'riscv64-unknown-elf'

    -ta | --toolchain-path              Optional. Specify the absolute path of toolchain
                                        if it is not present in PATH.

    -ip | --install-path                Optional. Path where Taurus will be installed.

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
            TAURUS_TOOLCHAIN_PREFIX="${2-}"
            shift
            ;;
        -ta | --toolchain-path)
            TAURUS_TOOLCHAIN_PATH="${2-}"
            shift
            ;;
        -ip | --install-path)
            TAURUS_INSTALL_PATH="${2-}"
            shift
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

if [[ -z ${TAURUS_TOOLCHAIN_PREFIX+x} ]]; then
    printf "Toolchain prefix not provided.\n"
    ERROR=1
fi

if [[ -z ${TAURUS_TARGET+x} ]]; then
    printf "Target not provided.\n"
    ERROR=1
elif [[ ! " ${VALID_TARGETS[*]} " =~ " $TAURUS_TARGET " ]]; then
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

Originally developed by
Centre for Development of Advanced Computing, India.

Forked and further developed by Debayan Sutradhar. All Rights Reserved.

To know full list of changes compared to the original SDK,
please visit
%s

This program comes with ABSOLUTELY NO WARRANTY.
This is free software, and you are welcome to redistribute it
under certain conditions.
    
Full license can be found in the 'LICENSE' file provided with the SDK.
The license can also be viewed by visiting %s

" "$VERSION" "$CHANGES_URL" "$LICENSE_URL"


com="cmake -B ${BUILD_DIR} -G \"${BUILD_SYSTEM}\" -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DTAURUS_TARGET=${TAURUS_TARGET} -DTAURUS_TOOLCHAIN_PREFIX=${TAURUS_TOOLCHAIN_PREFIX} "

if [[ ! -z ${TAURUS_TOOLCHAIN_PATH+x} ]]; then
    com+="-DTAURUS_TOOLCHAIN_PATH=${TAURUS_TOOLCHAIN_PATH}"
fi

printf "\nGenerate build system for %s\n" "$TAURUS_TARGET"

eval $com

if [[ $? -ne 0 ]]; then
    printf "Failed to generate build system for Taurus SDK.\n"
    exit 1
fi




printf "\nCompiling\n"

cmake --build $BUILD_DIR

if [[ $? -ne 0 ]]; then
    printf "Failed to compile Taurus SDK.\n"
    exit 1
fi




printf "\nInstalling\n"

if [[ -z ${TAURUS_INSTALL_PATH+x} ]] || [[ -r "${TAURUS_INSTALL_PATH}" ]]; then
    printf "\nAdditional permissions required. You might be asked for root password.\n\n"
    sudo make -C ${BUILD_DIR} install
else
    make -C ${BUILD_DIR} install
fi

if [[ $? -ne 0 ]]; then
    printf "Failed to install Taurus SDK.\n"
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
