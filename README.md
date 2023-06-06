# Taurus

This is my fork of [CDAC Vega](https://vegaprocessors.in/devboards) SDK with additional changes. 
It is an experiment and mostly for learning how such systems work, for now.

Contributions are welcome :)

## Platforms Supported

- Linux
- Windows

## Prerequisites

- CMake
- Ninja (Only on Windows)
- RISC-V GNU Compiler Toolchain

## Comparison with official SDK

- Usage of CMake instead of GNU Make: This allows usage of multiple build systems and compilation on Windows and MacOS alongside Linux.
- Integration with [vegadude](https://github.com/rnayabed/vegadude) uploader tool for seamless development experience.
- Cleaner code
- Examples moved to seperate [repository](https://github.com/rnayabed/taurus-examples).
- setup.sh: Completely rewritten with fine tunable features. See setup script usage for more information.
- gpio:
    - All GPIO pins are intialised as OUTPUT LOW. 
      Note: On Aries v2.0, Pin 26,27,29 and 30 are set to OUTPUT HIGH as these are inbuilt active-low LEDs.
- Removed led: Not needed as it is the same as gpio.

## TODO

- Avoid GNU extensions to allow usage of RISC-V LLVM toolchain
- Documentation

## How to use?

### Clone   

Clone the repository using the following command:
```
git clone https://github.com/rnayabed/taurus.git
```

### Build SDK (Linux)

Run the setup script with preferred options:

```
./taurus/setup.sh -t <TARGET> -tp <TOOLCHAIN PREFIX> -ta <TOOLCHAIN PATH>
```

- `<TARGET>`: Valid targets include `THEJAS32`, `THEJAS64` and `CDACFPGA`.

| Target   | Development Board                                  |
|----------|--------------------------------------------------- |
|`THEJAS32`| Aries v2.0, v3.0, Aries MICRO v1.0, Aries IoT v1.0 |
|`THEJAS64`|                                                    |
|`CDACFPGA`|                                                    |

- `<TOOLCHAIN PREFIX>`: RISC-V GNU Compiler Toolchain prefix.
- `<TOOLCHAIN PATH>`: Toolchain path. Use only if it is not present in `PATH`.

For more usage options, see setup script usage.

## Setup script usage

```
Usage:  [-tb | --target-board] [-ts | --target-soc]
        [-tt | --toolchain-triplet] [-tp | --toolchain-path]
        [-ip | --install-path] [-vp | --vegadude-path]
        [-nm | --no-minicom]
        [-h | --help]

Option Summary:
    -tb | --target-board                Required if --target-soc not provided.
                                        Set the target development board to
                                        build Taurus for. Adds extra optimisations
                                        for board if available.
                                        Valid targets are:
                                        ARIES_V2 ARIES_V3 ARIES_MICRO_V1 ARIES_IOT_V1

    -ts | --target-soc                  Required if --target-board not provided.
                                        Set the target System-on-Chip to build
                                        Taurus for.
                                        Valid targets are:
                                        THEJAS32 THEJAS64 CDACFPGA
    
    -tt | --toolchain-triplet           Required. RISC-V GNU Compiler Toolchain
                                        triplet.
                                        Example: 'riscv64-unknown-elf'

    -tp | --toolchain-path              Optional. Specify the absolute path of
                                        toolchain if it is not present in PATH.

    -ip | --install-path                Optional. Path where Taurus will be
                                        installed.

    -vp | --vegadude-path               Optional. Provide vegadude path for taurus integration.
                                        Not required if vegadude is already present in PATH.

    -nm | --no-minicom                  Optional. Do not create minicom
                                        configuration file. Configuration is
                                        created if not specified.

    -h  --help                          Print this message.
```

## Credits

Originally developed by [Centre for Development of Advanced Computing, India](https://www.cdac.in/).

Forked and further developed by [Debayan Sutradhar](https://github.com/rnayabed) & 
[Avra Mitra](https://github.com/abhra0897). All Rights Reserved.

## License

Taurus is licensed under the [MIT License](https://github.com/rnayabed/taurus/blob/master/LICENSE).
