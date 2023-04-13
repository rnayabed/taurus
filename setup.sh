#!/bin/bash

TAURUS_SDK=`pwd`

echo "Setting up Taurus SDK Environment"

echo "# Tarus SDK - Global Configuration
TAURUS_SDK=$TAURUS_SDK
TAURUS_COMPILER_PREFIX=riscv64-unknown-elf
VEGA_MACHINE=THEJAS32
" > ~/.config/vega-tools/settings.mk

make clean
make

echo "Configuring minicom & adding user to 'dialout' group"
echo "# Taurus SDK - Minicom Configuration for CDAC Aries 
pu port             /dev/ttyUSB0
pu baudrate         115200
pu bits             8
pu parity           N
pu stopbits         1
pu updir            $TAURUS_SDK/bin
pu rtscts           No 
" | sudo tee /etc/minirc.aries > /dev/null

sudo usermod -a -G dialout $USER
echo "---------------------------------------------------------------------"
echo "Minicom configured for THEJAS32 hardware & user added to 'dialout' group."
echo "Run 'minicom aries' to open terminal to it."
echo "---------------------------------------------------------------------"

echo "VEGA SDK Environment added"