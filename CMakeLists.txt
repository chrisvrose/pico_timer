# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.14)

# For clangd support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# explicitly for the pico w
# set(PICO_BOARD pico_w)

# Include build functions from Pico SDK
# Someday, support relative sdk placement
set(PICO_SDK_PATH /home/atreyab/Buildspace/pico-sdk)
# set(PICO_SDK_PATH ${CMAKE_SOURCE_DIR}/libs/pico/pico-sdk)

include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C   standards
project(piclock C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

add_subdirectory(libs/ssd1306/pico-ssd1306)
add_subdirectory(libs/pico_dht/dht)


## Executable 1 - clock

# Tell CMake where to find the executable source file
add_executable(clock
    src/main.cpp
    src/context.cpp
    src/display.cpp
    src/temp_dht.cpp
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(clock)

target_include_directories(clock PRIVATE
    libs/ssd1306
    libs/pico_dht/dht/include
)

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(clock
    pico_stdlib
    pico_ssd1306
    hardware_i2c
    hardware_rtc
    pico_status_led
    dht
    # pico_cyw43_arch_none
)

# Enable usb output, disable uart output
pico_enable_stdio_usb(clock 1)
pico_enable_stdio_uart(clock 0)


## Executable 2 - scan_i2c - Helper executable to probe and find i2c devices

# Tell CMake where to find the executable source file
add_executable(scan_i2c
    src/scan_i2c.cpp
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(scan_i2c)


# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(scan_i2c
    pico_stdlib
    hardware_i2c
)

# Enable usb output, disable uart output
pico_enable_stdio_usb(scan_i2c 1)
pico_enable_stdio_uart(scan_i2c 0)
