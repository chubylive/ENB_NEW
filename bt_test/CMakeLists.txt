cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ../Platform/LPC1769.cmake)

project(bt_test C)

include_directories(
BT_Stack_inc/
liblpc-inc/
)

set(SOURCES
 src/cr_startup_lpc176x.c
 src/main.c


 # Lib LPC Peripheral Drivers
 liblpc-src/lpc17xx_clkpwr.c
 liblpc-src/lpc17xx_pinsel.c
 liblpc-src/lpc17xx_libcfg_default.c
 liblpc-src/lpc17xx_timer.c
 liblpc-src/lpc17xx_uart.c


 #BT_Stack Source Files
 BT_Stack_src/btstack_memory.c
 BT_Stack_src/hal_tick.c
 BT_Stack_src/hal_uart_dma.c
 BT_Stack_src/hci.c
 BT_Stack_src/hci_cmds.c
 BT_Stack_src/hci_dump.c
 BT_Stack_src/hci_transport_h4_dma.c
 BT_Stack_src/linked_list.c
 BT_Stack_src/memory_pool.c
 BT_Stack_src/remote_device_db_memory.c
 BT_Stack_src/run_loop.c
 BT_Stack_src/run_loop_embedded.c
 BT_Stack_src/utils.c
 BT_Stack_src/bluetooth_init_cc2564_2.10.c
 BT_Stack_src/bt_control_cc256x.c
 



)

# Debug builds by default, uncomment for Release:
# set(CMAKE_RELEASE True)

# Uncomment to enable Semihosting (required to even link against
# semihosting libraries)
set(SEMIHOSTING_ENABLED True)

include(../Platform/LPC1769_project_default.cmake)
include(../Platform/LPC1769_targets.cmake)
