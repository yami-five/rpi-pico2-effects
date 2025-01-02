# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PICO_DEOPTIMIZED_DEBUG ON)
set(CMAKE_BUILD_TYPE Debug)
set(PICO_DEFAULT_FLOAT_IMPL pico)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(demo C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
add_subdirectory(lib/Config)
add_subdirectory(lib/effects)
add_subdirectory(lib/rasterizer)
add_subdirectory(lib/painter)
add_subdirectory(lib/lcd)
include_directories(./lib/Config)
include_directories(./lib/effects)
include_directories(./lib/rasterizer)
include_directories(./lib/painter)
include_directories(./lib/lcd)


add_executable(demo demo.c )

pico_set_program_name(demo "demo")
pico_set_program_version(demo "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(demo 1)
pico_enable_stdio_usb(demo 0)

target_link_libraries(demo
        pico_stdlib Config effects rasterizer painter lcd)

# Add the standard include files to the build
target_include_directories(demo PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any user requested libraries
target_link_libraries(demo 
        hardware_spi
        hardware_i2c
        hardware_pwm
        )

pico_add_extra_outputs(demo)
