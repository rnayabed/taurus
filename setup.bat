:: SPDX-License-Identifier: MIT
:: Windows Setup Script
:: Authors : Debayan Sutradhar (@rnayabed)

@echo off

set "VERSION=1.0"

set "WEBSITE_URL=https://github.com/rnayabed/taurus.git"
set "LICENSE_URL=https://github.com/rnayabed/taurus/blob/master/LICENSE"
set "CHANGES_URL=https://github.com/rnayabed/taurus/blob/master/README.md#comparison-with-official-sdk"

set "SOURCE_PATH=%~dp0"
set "SOURCE_PATH=%SOURCE_PATH:\=/%"
set "BUILD_PATH=%SOURCE_PATH%/build"
set "BUILD_TYPE=Debug"
set "BUILD_SYSTEM=Ninja"

set "VALID_TARGET_BOARDS=ARIES_V2 ARIES_V3 ARIES_MICRO_V1 ARIES_IOT_V1"
set "VALID_TARGET_SOCS=THEJAS32 THEJAS64 CDACFPGA"


set "TAURUS_TARGET_BOARD="
set "TAURUS_TARGET_SOC="
set "TAURUS_TOOLCHAIN_TRIPLET="
set "TAURUS_TOOLCHAIN_PATH="
set "TAURUS_INSTALL_PATH="
set "TAURUS_VEGADUDE_PATH="

for %%A in (%*) do (
    goto :parse_args
)

call :usage
exit /b 1


:parse_args

if "%~1"=="-tb"                     ( goto :set_target_board )
if "%~1"=="--target-board"          ( goto :set_target_board )

if "%~1"=="-ts"                     ( goto :set_target_soc )
if "%~1"=="--target-soc"            ( goto :set_target_soc )

if "%~1"=="-tt"                     ( goto :set_toolchain_triplet )
if "%~1"=="--toolchain-triplet"     ( goto :set_toolchain_triplet )

if "%~1"=="-tp"                     ( goto :set_toolchain_path )
if "%~1"=="--toolchain-path"        ( goto :set_toolchain_path )

if "%~1"=="-ip"                     ( goto :set_install_path )
if "%~1"=="--install-path"          ( goto :set_install_path )

if "%~1"=="-vp"                     ( goto :set_vegadude_path )
if "%~1"=="--vegadude-path"         ( goto :set_vegadude_path )

if not "%~1"=="" (
    echo Invalid option %~1
    echo Run with --help for usage.
    exit /b 1
)

goto :validate_inputs

:parse_args_tail
shift
shift
goto :parse_args

:validate_inputs

set ERROR=0

set TARGET_ARGS=0
if not "%TAURUS_TARGET_BOARD%"==""  ( set /A TARGET_ARGS+=1 )
if not "%TAURUS_TARGET_SOC%"==""    ( set /A TARGET_ARGS+=1 )

if %TARGET_ARGS% equ 0 (
    echo Target board or target SoC required.
    set /A ERROR = 1
)
if %TARGET_ARGS% equ 2 (
    echo You cannot provide target board and target SoC at the same time.
    set /A ERROR = 1
)

if not "%TAURUS_TARGET_BOARD%"=="" (
    for %%A in (%VALID_TARGET_BOARDS%) do (
        if "%TAURUS_TARGET_BOARD%"=="%%A" ( goto :validate_inputs_tail )
    )
    echo Invalid target board provided.
    echo Valid target boards are %VALID_TARGET_BOARDS%
    set /A ERROR = 1
) else (
    for %%A in (%VALID_TARGET_SOCS%) do (
        if "%TAURUS_TARGET_SOC%"=="%%A" ( goto :validate_inputs_tail )
    )
    echo Invalid target SoC provided.
    echo Valid target SoCs are %VALID_TARGET_SOCS%
    set /A ERROR = 1
)

:validate_inputs_tail
if %ERROR% equ 1 (
    echo Run with --help for usage.
    exit /b 1
)


echo The args are
echo TAURUS_TARGET_BOARD: %TAURUS_TARGET_BOARD%
echo TAURUS_TARGET_SOC: %TAURUS_TARGET_SOC%
echo TAURUS_TOOLCHAIN_TRIPLET: %TAURUS_TOOLCHAIN_TRIPLET%
echo TAURUS_TOOLCHAIN_PATH: %TAURUS_TOOLCHAIN_PATH%
echo TAURUS_INSTALL_PATH: %TAURUS_INSTALL_PATH%
echo TAURUS_VEGADUDE_PATH: %TAURUS_VEGADUDE_PATH%

echo:
echo:
echo *****               *****
echo   ****             ****
echo     ****         ****
echo      *****     *****
echo        ************
echo       **************
echo      ***         ***
echo     ***           ***
echo     ***           ***
echo      ***         ***
echo       *****   *****
echo         *********
echo:
echo     Taurus SDK - %VERSION%
echo:
echo:
echo Website
echo %WEBSITE_URL%
echo:
echo To know full list of changes compared to the original SDK,
echo please visit
echo %CHANGES_URL%
echo:
echo This program comes with ABSOLUTELY NO WARRANTY.
echo This is free software, and you are welcome to redistribute it
echo under certain conditions.
echo:
echo Full license can be found in the 'LICENSE' file provided with the SDK.
echo The license can also be viewed by visiting %LICENSE_URL%
echo:

set "com=cmake -B "%BUILD_PATH%" -S "%SOURCE_PATH%" -G "%BUILD_SYSTEM%" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DTAURUS_TOOLCHAIN_TRIPLET=%TAURUS_TOOLCHAIN_TRIPLET% "

if not "%TAURUS_TARGET_BOARD%"=="" (
    set "com=%com%-DTAURUS_TARGET_BOARD=%TAURUS_TARGET_BOARD% "
    set "TAURUS_TARGET=%TAURUS_TARGET_BOARD%"
) else (
    set "com=%com%-DTAURUS_TARGET_SOC=%TAURUS_TARGET_SOC% "
    set "TAURUS_TARGET=%TAURUS_TARGET_SOC%"
)

if not "%TAURUS_TOOLCHAIN_PATH%"=="" (
    set "com=%com%-DTAURUS_TOOLCHAIN_PATH="%TAURUS_TOOLCHAIN_PATH%" "
)

if not "%TAURUS_INSTALL_PATH%"=="" (
    set "com=%com%-DCMAKE_INSTALL_PREFIX="%TAURUS_INSTALL_PATH%" "
)

if not "%TAURUS_VEGADUDE_PATH%"=="" (
    set "com=%com%-DTAURUS_VEGADUDE_PATH="%TAURUS_VEGADUDE_PATH%" "
)

echo Removing old files ...

rmdir /S /Q "%BUILD_PATH%"

echo Generating build system ...

%com% || goto :failed_to_generate_build_system

echo Compiling ...

cmake --build "%BUILD_PATH%" || goto :failed_to_compile

echo Installing ...

cmake --install "%BUILD_PATH%" || goto :failed_to_install

echo:
echo =====================================================================
echo Taurus SDK %VERSION% for %TAURUS_TARGET% is now ready to use.
echo:
echo For queries, issues, etc. visit
echo %WEBSITE_URL%
echo:
echo Enjoy!
echo =====================================================================

exit /b 0





:failed_to_generate_build_system
echo:
echo Failed to generate build system for Taurus SDK.
exit /b 1

:failed_to_compile
echo:
echo Failed to compile Taurus SDK.
exit /b 1

:failed_to_install
echo:
echo Failed to install Taurus SDK. Check if you have required rights.
echo Rerun script with administrator rights if you are installing in a sensitive directory.
exit /b 1

:usage
echo Usage:  ^[-tb ^| --target-board ^] ^[-ts ^| --target-soc^]
echo         ^[-tt ^| --toolchain-triplet^] ^[-tp ^| --toolchain-path^]
echo         ^[-ip ^| --install-path^] ^[-vp ^| --vegadude-path^]
echo         ^[-h ^| --help^]
echo:
echo Option Summary:
echo    -tb ^| --target-board                Required if --target-soc not provided.
echo                                        Set the target development board to
echo                                        build Taurus for. Adds extra optimisations
echo                                        for board if available.
echo                                        Valid targets are:
echo                                        %s
echo:
echo    -ts ^| --target-soc                  Required if --target-board not provided.
echo                                        Set the target System-on-Chip to build
echo                                        Taurus for.
echo                                        Valid targets are:
echo                                        %s
echo:
echo    -tt ^| --toolchain-triplet           Required. RISC-V GNU Compiler Toolchain
echo                                        triplet.
echo                                        Example: 'riscv64-unknown-elf'
echo:
echo    -tp ^| --toolchain-path              Optional. Specify the absolute path of
echo                                        toolchain if it is not present in PATH.
echo:
echo    -ip ^| --install-path                Optional. Path where Taurus will be
echo                                        installed.
echo:
echo    -vp ^| --vegadude-path               Optional. Provide vegadude path for taurus integration.
echo                                        Not required if vegadude is already present in PATH.
echo:
echo    -h  --help                          Print this message.
echo:
exit /b 0

:set_target_board
set "TAURUS_TARGET_BOARD=%~2"
goto :parse_args_tail

:set_target_soc
set "TAURUS_TARGET_SOC=%~2"
goto :parse_args_tail

:set_toolchain_triplet
set "TAURUS_TOOLCHAIN_TRIPLET=%~2"
goto :parse_args_tail

:set_toolchain_path
set "TAURUS_TOOLCHAIN_PATH=%~2"
set "TAURUS_TOOLCHAIN_PATH=%TAURUS_TOOLCHAIN_PATH:\=/%"
goto :parse_args_tail

:set_install_path
set "TAURUS_INSTALL_PATH=%~2"
set "TAURUS_INSTALL_PATH=%TAURUS_INSTALL_PATH:\=/%"
goto :parse_args_tail

:set_vegadude_path
set "TAURUS_VEGADUDE_PATH=%~2"
set "TAURUS_VEGADUDE_PATH=%TAURUS_VEGADUDE_PATH:\=/%"
goto :parse_args_tail
