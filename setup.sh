#!/bin/bash

# SPDX-License-Identifier: MIT
# Authors : Debayan Sutradhar (@rnayabed)

VERSION=1.0
TAURUS_SDK=`dirname -- $(readlink -f "${BASH_SOURCE}")`

CREATE_MINICOM_CONFIG=1
MINICOM_CONFIG=/etc/minirc.aries

REPO_URL=https://github.com/rnayabed/taurus.git
LICENSE_URL=https://github.com/rnayabed/taurus/blob/master/LICENSE
CHANGES_URL=https://github.com/rnayabed/taurus/blob/master/README.md#comparison-with-official-sdk

BUILD_DIR=build
BUILD_TYPE=Debug
BUILD_SYSTEM="Unix Makefiles"

VALID_TARGET_BOARDS=("ARIES_V2" "ARIES_V3" "ARIES_MICRO_V1" "ARIES_IOT_V1")
VALID_TARGET_SOCS=("THEJAS32" "THEJAS64" "CDACFPGA")


usage() {
  printf "
Usage:  [-tb | --target-board] [-ts | --target-soc]
        [-tt | --toolchain-triplet] [-ta | --toolchain-path]
        [-ip | --install-path] [-vp | --vegadude-path]
        [-nm | --no-minicom]
        [-h | --help]

Option Summary:
    -tb | --target-board                Required if --target-soc not provided.
                                        Set the target development board to
                                        build Taurus for. Adds extra optimisations
                                        for board if available.
                                        Valid targets are:
                                        %s

    -ts | --target-soc                  Required if --target-board not provided.
                                        Set the target System-on-Chip to build
                                        Taurus for.
                                        Valid targets are:
                                        %s
    
    -tt | --toolchain-triplet           Required. RISC-V GNU Compiler Toolchain
                                        triplet.
                                        Example: 'riscv64-unknown-elf'

    -ta | --toolchain-path              Optional. Specify the absolute path of
                                        toolchain if it is not present in PATH.

    -ip | --install-path                Optional. Path where Taurus will be
                                        installed.

    -vp | --vegadude-path               Optional. Provide vegadude path for taurus integration.
                                        Not required if vegadude is already present in PATH.

    -nm | --no-minicom                  Optional. Do not create minicom
                                        configuration file. Configuration is
                                        created if not specified.

    -h  --help                          Print this message.
" "${VALID_TARGET_BOARDS[*]}" "${VALID_TARGET_SOCS[*]}"
}

if [ $# -eq 0 ]; then
usage
exit 1
fi

while :; do
    case "${1-}" in
    -tb | --target-board)
        TAURUS_TARGET_BOARD="${2-}"
        shift
        ;;
    -ts | --target-soc)
        TAURUS_TARGET_SOC="${2-}"
        shift
        ;;
    -tt | --toolchain-triplet)
        TAURUS_TOOLCHAIN_TRIPLET="${2-}"
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
    -vp | --vegadude-path)
        TAURUS_VEGADUDE_PATH="${2-}"
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

if [[ ! -z ${TAURUS_TARGET_BOARD+x} ]] && [[ ! -z ${TAURUS_TARGET_SOC+x} ]]; then
    printf "You cannot provide target board and target SoC at the same time.\n"
    ERROR=1
elif [[ -z ${TAURUS_TARGET_BOARD+x} ]] && [[ -z ${TAURUS_TARGET_SOC+x} ]]; then
    printf "Target board or target SoC required.\n"
    ERROR=1
fi

if [[ ! -z ${TAURUS_TARGET_BOARD+x} ]] && [[ ! " ${VALID_TARGET_BOARDS[*]} " =~ " $TAURUS_TARGET_BOARD " ]]; then
    printf "Invalid target board provided.
Valid target boards are %s\n" "${VALID_TARGET_BOARDS[*]}"
    ERROR=1
fi

if [[ ! -z ${TAURUS_TARGET_SOC+x} ]] && [[ ! " ${VALID_TARGET_SOCS[*]} " =~ " $TAURUS_TARGET_SOC " ]]; then
    printf "Invalid target SoC provided.
Valid target SoCs are %s\n" "${VALID_TARGET_SOCS[*]}"
    ERROR=1
fi

if [[ -z ${TAURUS_TOOLCHAIN_TRIPLET+x} ]]; then
    printf "Toolchain triplet required.\n"
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

Website
%s

To know full list of changes compared to the original SDK,
please visit
%s

This program comes with ABSOLUTELY NO WARRANTY.
This is free software, and you are welcome to redistribute it
under certain conditions.
    
Full license can be found in the 'LICENSE' file provided with the SDK.
The license can also be viewed by visiting %s

" "$VERSION" "$REPO_URL" "$CHANGES_URL" "$LICENSE_URL"


com="cmake -B ${BUILD_DIR} -G \"${BUILD_SYSTEM}\" -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DTAURUS_TOOLCHAIN_TRIPLET=${TAURUS_TOOLCHAIN_TRIPLET} "

if [[ ! -z ${TAURUS_TARGET_BOARD+x} ]]; then
    com+="-DTAURUS_TARGET_BOARD=${TAURUS_TARGET_BOARD} "
else
    com+="-DTAURUS_TARGET_SOC=${TAURUS_TARGET_SOC} "
fi

if [[ ! -z ${TAURUS_TOOLCHAIN_PATH+x} ]]; then
    com+="-DTAURUS_TOOLCHAIN_PATH=${TAURUS_TOOLCHAIN_PATH}"
fi

if [[ ! -z ${TAURUS_INSTALL_PATH+x} ]]; then
    com+="-DCMAKE_INSTALL_PREFIX=${TAURUS_INSTALL_PATH}"
fi

if [[ ! -z ${TAURUS_VEGADUDE_PATH+x} ]]; then
    com+="-DTAURUS_VEGADUDE_PATH=${TAURUS_VEGADUDE_PATH}"
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
