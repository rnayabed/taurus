#!/bin/bash

# FIXME: Move to printf from echo

VERSION=1.0
VEGA_TARGET='THEJAS32'
CREATE_MINICOM_CONFIG=1
TAURUS_SDK=`pwd`
GLOBAL_MAKE_FILE=~/.config/vega-tools/settings.mk
LICENSE_URL=https://github.com/rnayabed/taurus-sdk/blob/master/LICENSE

usage() {
  cat << EOF
Usage:  [-t | --target] [-nm | --no-minicom]
        [-h | --help]

Note:
Target will be assumed to be $DEFAULT_TARGET if not specified.
Minicom configuration file will be created if not specified.

Option Summary:
    [-t | --target]                     Set the target to build SDK for. 
                                        Possible values are:
                                        'THEJAS32' (Aries v2.0, v3.0, Micro v1.0, IoT v1.0)
                                        'THEJAS64'
                                        'CDACFPGA'
    
    [-nm | --no-minicom]                Do not create minicom configuration file.

    -h  --help                          Print this message.
EOF
}

parse_params() {
    while :; do
        case "${1-}" in
        -t | --target)
            VEGA_TARGET="${2-}"
            shift
            ;;
        -nm | --no-minicom)
            CREATE_MINICOM_CONFIG=0
            ;;
        *) 
            if [ ! -z "${1-}" -a "${1-}" != " " ]; then
                usage 
                exit 0
            else
                break
            fi
            ;;
        esac
        shift
    done
} 



cat << EOF

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
          v$VERSION

Copyright (C) 2023 Debayan Sutradhar

This program comes with ABSOLUTELY NO WARRANTY.
This is free software, and you are welcome to redistribute it
under certain conditions.
    
Full license can be found in the "LICENSE" file provided with the SDK.
The license can also be viewed by visiting $LICENSE_URL


EOF



parse_params "$@"



echo 'Setting up Global Make file'
echo "# Tarus SDK - Global Configuration
TAURUS_SDK=$TAURUS_SDK
TAURUS_COMPILER_PREFIX=riscv64-unknown-elf
VEGA_TARGET=$VEGA_TARGET
" > $GLOBAL_MAKE_FILE



echo
echo "Compiling Taurus SDK for $VEGA_TARGET"
make

if [ $? -ne 0 ]; then
    echo 'Failed to compile Taurus SDK.'
    exit 1
fi


if [ "$CREATE_MINICOM_CONFIG" -eq 1 ]; then
    echo
    echo 'Creating Minicom configuration file'
    echo "# Taurus SDK - Minicom Configuration for CDAC Vega
pu port             /dev/ttyUSB0
pu baudrate         115200
pu bits             8
pu parity           N
pu stopbits         1
pu updir            $TAURUS_SDK/bin
pu rtscts           No 
" | sudo tee /etc/minirc.aries > /dev/null

if [ $? -ne 0 ]; then
    echo 'Failed to create Minicom configuration file'
    exit 1
fi

fi



echo
echo '====================================================================='
echo "Taurus SDK $VERSION for $VEGA_TARGET is now ready to use."
echo "You can use the makefile stored at $GLOBAL_MAKE_FILE."

if [ "$CREATE_MINICOM_CONFIG" -eq 1 ]; then
    echo
    echo 'Minicom configuration has also been created.'
    echo 'You can access it by running "sudo minicom aries".'
    echo 'You may also add yourself in the "dialout" user group to use'
    echo 'minicom without root permissions.'
fi

echo
echo 'Enjoy!'
echo '====================================================================='