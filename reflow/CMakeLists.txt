cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ../Platform/LPC1769.cmake)

project(Reflow C)

include_directories(
	liblpc-inc/
	inc/
	MAX31885_lib/
)

set(SOURCES
 src/cr_startup_lpc176x.c
 src/main.c
 MAX31885_lib/max_lib.c
 MAX31885_lib/spi.c
 
)

# Debug builds by default, uncomment for Release:
# set(CMAKE_RELEASE True)

# Uncomment to enable Semihosting (required to even link against
# semihosting libraries)
set(SEMIHOSTING_ENABLED True)

include(../Platform/LPC1769_project_default.cmake)
include(../Platform/LPC1769_targets.cmake)
