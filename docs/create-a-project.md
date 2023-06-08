# Create a project

Taurus uses CMake.

Let's create a simple blink project.

Our project will have the following directory tree
```
.
├── CMakeLists.txt
└── led_toggle.c
```

##

### CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.16)

find_package(taurus 0.1 REQUIRED)

project(led_toggle
    LANGUAGES C)

add_executable(${PROJECT_NAME}
    led_toggle.c)

target_link_libraries(${PROJECT_NAME}
    PRIVATE taurus::taurus)

configure_taurus_uploader(
    START_AFTER_UPLOAD
    TARGET_PATH /dev/ttyUSB0)
```


The `configure_taurus_uploader` is a custom function provided by the Taurus SDK. 
It integrates your project with [vegadude](https://github.com/rnayabed/vegadude.git) so that you can easily upload programs
directly from CMake.

`configure_taurus_uploader` as additional options:

```
configure_taurus_uploader(
    [START_AFTER_UPLOAD - flag if you immediately want to start program after upload] 
    EXEC_NAME <name of the executable - by default the project name>
    TARGET_PATH <path to your target. For example COM3 on windows, /dev/ttyUSB0 on Linux>
    VEGADUDE_ARGS <additional arguments that can be passed to vegadude>
```

For detailed usage of vegadude, check it's [usage](https://github.com/rnayabed/vegadude#usage).

### led_toggle.c

```cpp
#include <taurus/stdlib.h>
#include <taurus/gpio.h>
#include <taurus/timer.h>

int current_status = LOW;

void timer0_interrupt_handler()
{
    TIMER_acknowledge_interrupt(TIMER_0);

    if (current_status == LOW)
    {
        printf("LED ON\n");
        current_status = HIGH;
    }
    else
    {
        printf("LED OFF\n");
        current_status = LOW;
    }

    GPIO_write_pin(GPIO_25, current_status);
}

void main()
{
    TIMER_setup_interrupt_seconds(TIMER_0, 1, timer0_interrupt_handler);
}
```

We use hardware timers with interrupts to blink an LED.
You could use busy looping using `TIMER_setup_delay_seconds(unsigned char timer, unsigned long seconds)` as well.

## Build and Run

Just like any usual CMake project, just run:

```
cmake -B build
```

To build, run:

```
cmake --build build
```

To upload the program on your Aries board, run:

```
cmake --install build
```

You will then get an output like this:

```
$> cmake --install build
-- Install configuration: ""
vegadude 1.0
<https://github.com/rnayabed/vegadude>

================================================
Device Path: "/dev/ttyUSB0"
Binary Path: "/home/rnayabed/code/rnayabed/taurus-examples/gpio/led_toggle/build/led_toggle"
Target device properties:
Parity: 0
Stop bits: 1
RTS CTS: 0
Bits: 8
Baud rate: 115200
Read Timeout (in milliseconds): 500
XMODEM Block Size 128
XMODEM Max Retry: 10
================================================

Progress [##################################################] - 100% - Sent block 213/213
Successfully uploaded and started program!
```

If you have an LED connected to GPIO 25, it will blink with a duration of 1 second.

If you use Linux, Taurus creates a minicom configuration by default unless `--no-minicom` is used during it's setup.

If you have minicom installed, you can run `sudo minicom aries` and the output would something like this:
```
LED ON
LED OFF
LED ON
LED OFF
LED ON
LED OFF
LED ON
LED OFF
LED ON
LED OFF
...
``` 

If you are on Windows, you can use any terminal with serial support.
