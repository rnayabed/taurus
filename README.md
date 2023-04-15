# Taurus SDK

This is my fork of [CDAC Aries](https://vegaprocessors.in/devboards) SDK with additional changes. It is an experiment and mostly for learning how such systems work, for now.

## Prerequisites

- GNU Autoconf
- GNU Make
- RISC-V GNU Compiler Toolchain (Newlib)
- Minicom

## Comparison with official SDK
- Different and cleaner workflow: Taurus is built during setup and not during external project compilation.
- Examples moved to seperate [repository](https://github.com/rnayabed/taurus-examples).
- Rewritten following aspects:
    - setup.sh 
    - Makefiles
    - GPIO: All pins are initialised as OUTPUT LOW to prevent Pin 26,27,29,30 to light up when not in use. Unknown if it is limited to my board or a hardware bug.
    - UART
- Removed items:
    - led: Not needed as it is the same as GPIO.

## Credits

Originally developed by [Centre for Development of Advanced Computing, India](https://www.cdac.in/).

Forked and further developed by [Debayan Sutradhar](https://github.com/rnayabed). All Rights Reserved.

## License

Taurus SDK is licensed to [GNU General Public License v3.0](https://github.com/rnayabed/taurus-sdk/blob/master/LICENSE).

```
Taurus SDK - Software Development Kit for CDAC Vega
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