# Taurus

This is my fork of [CDAC Vega](https://vegaprocessors.in/devboards) SDK with additional changes. It is an experiment and mostly for learning how such systems work, for now.

## Prerequisites

- CMake
- RISC-V GNU Compiler Toolchain (Newlib)
- Minicom

## Comparison with official SDK

- Usage of CMake instead of GNU Make: This allows usage of multiple build systems and compilation on Windows and MacOS alongside Linux.
- The build output is stored in a seperate `build` directory instead of storing directly in the same directory.
- Examples moved to seperate [repository](https://github.com/rnayabed/taurus-examples).
- setup.sh: Completely rewritten with fine tunable features. See setup script usage for more information.
- gpio: All pins are initialised as OUTPUT LOW to prevent Pin 26,27,29,30 to light up when not in use. Unknown if it is limited to my board or a hardware bug.
- Removed led: Not needed as it is the same as gpio.

## TODO

- Provide option to download prebuilt RISC-V GCC toolchain directly
- Create setup script for Windows
- Create all-in-one tool to upload programs to Aries boards
- Allow LLVM toolchain to be used instead of only GCC
- Avoid GNU extensions to allow usage of RISC-V LLVM toolchain
- Documentation

## How to use?

### Clone   

Clone the repository using the following command:
```
git clone https://github.com/rnayabed/taurus-sdk.git
```

### Build SDK (Linux)

Run the setup script with preferred options:

```
./taurus-sdk/setup.sh -t <TARGET> -tp <TOOLCHAIN PREFIX> -ta <TOOLCHAIN PATH>
```

- `<TARGET>`: Valid targets include `THEJAS32`, `THEJAS64` and `CDACFPGA`.

| Target   | Development Board                      |
|----------|----------------------------------------|
|`THEJAS32`| Aries v2.0, v3.0, MICRO v1.0, IoT v1.0 |
|`THEJAS64`|                                        |
|`CDACFPGA`|                                        |

- `<TOOLCHAIN PREFIX>`: RISC-V GNU Compiler Toolchain prefix.
- `<TOOLCHAIN PATH>`: Toolchain path. Use only if it is not present in `PATH`.

For more usage options, see setup script usage.

## Setup script usage

```
Usage:  [-t | --target]
        [-tp | --toolchain-prefix] [-ta | --toolchain-path]
        [-ip | --install-path]
        [-nm | --no-minicom] [-h | --help]

Option Summary:
    -t | --target                       Required. Set the target to build SDK for.
                                        Valid targets are:
                                        THEJAS32 THEJAS64 CDACFPGA

    -tp | --toolchain-prefix            Required. RISC-V GNU Compiler Toolchain prefix.
                                        Example: 'riscv64-unknown-elf'

    -ta | --toolchain-path              Optional. Specify the absolute path of toolchain
                                        if it is not present in PATH.

    -ip | --install-path                Optional. Path where Taurus will be installed.

    -nm | --no-minicom                  Optional. Do not create minicom configuration file.
                                        Configuration is created if not specified.

    -h  --help                          Print this message.
```

## Credits

Originally developed by [Centre for Development of Advanced Computing, India](https://www.cdac.in/).

Forked and further developed by [Debayan Sutradhar](https://github.com/rnayabed). All Rights Reserved.

## License

Taurus is licensed to [GNU General Public License v3.0](https://github.com/rnayabed/taurus-sdk/blob/master/LICENSE).

```
Taurus - Software Development Kit for CDAC Vega
Copyright (C) 2023 Debayan Sutradhar (rnayabed)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
```
