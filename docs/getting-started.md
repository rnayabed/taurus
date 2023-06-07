# Getting Started

## Prerequisites

##### **All of the following prerequisites must be available in your PATH.**

- [CMake](https://cmake.org/)
- RISC-V GNU Toolchain (Newlib) with support for `rv32im`. 
    - For Windows, you can use [this prebuilt toolchain](https://gnutoolchains.com/risc-v/).
    - For Linux, you can use [this prebuilt toolchain](https://github.com/stnolting/riscv-gcc-prebuilt) or refer to your distribution's repositories.
- [vegadude](https://github.com/rnayabed/vegadude/). This is the uploader tool to upload your program to your Aries development board. It let's you upload your program directly from cmake via `cmake --install` without having to use a 3rd party terminal tool.
- [Ninja](https://ninja-build.org/) **: For Windows Only. Linux systems do not require this.**

## Terminal emulator

For terminal, minicom is recommended on Linux. 
Taurus creates a minicom configuration for aries boards by default for ease of use, which you can access by simply running `sudo minicom aries`.

You can also choose to use a different terminal emulator.

## Steps

- Download and extract the [latest release of Taurus](https://github.com/rnayabed/taurus/releases/latest)
- On Windows, run:
```
setup.bat --target-triplet <Target Triplet> --target-board <Target Board>
```

**Run the commmand prompt as Administrator if you want to install Taurus SDK in the default directory (`C:\Program Files (x86)\taurus`).
You can use `--install-path <path>` to specify where you would like to install.**
- On Linux, run:
```
./setup.sh --target-triplet <Target Triplet> --target-board <Target Board>
```

- `<Target Triplet>`: Target triplet from the RISC-V GNU Toolchain. 
If you are using a prebuilt toolchain, go to your toolchain directory, there will be a folder with the format `riscv64-<vendor>-elf`. For example, `riscv64-unknown-elf`.
- `<Target Board>`: This is the board you are using. 
Available options are: `ARIES_V2`, `ARIES_V3`, `ARIES_MICRO_V1`, `ARIES_IOT_V1`.
- If you are using a different board, you can also use the option `--target-soc` and specify the SoC name.
Available options are: `THEJAS32`, `THEJAS64` and `CDACFPGA`.
- The setup script offers a lot of fine tunable options. you can run the script `--help` for detailed usage.

## Finishing Up

After running the script, you will be greeted with a success message. 
If you are using Linux, it will also create a minicom configuration for ease of use. You can skip this by running the setup script with `--no-minicom`

If you have installed Taurus in a custom location, you need to add it to PATH, or use `-DCMAKE_PREFIX_PATH=<path>` while setting up a Taurus powered project.

If you are getting errors, please create a [GitHub issue](https://github.com/rnayabed/taurus/issues/new).

Otherwise, you can proceed to [create project using Taurus](https://github.com/rnayabed/taurus/blob/master/docs/create-a-project.md), or compiling and uploading [examples](https://github.com/rnayabed/taurus-examples.git)
