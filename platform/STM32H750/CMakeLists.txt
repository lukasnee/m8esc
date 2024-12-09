project(STM32H750)

set(ROOT ${CMAKE_SOURCE_DIR})

# ##############################################################################
# Platform-specific Includes and Sources
# ##############################################################################

set(PLATFORM_SPECIFIC_INCLUDES_STM32H7xx
    Core/Inc
    USB_HOST/App
    USB_HOST/Target
    Drivers/STM32H7xx_HAL_Driver/Inc
    Drivers/STM32H7xx_HAL_Driver/Inc/Legacy
    Middlewares/ST/STM32_USB_Host_Library/Core/Inc
    Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Inc
    Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc
    ../../extern/ILI9341-STM32-HAL
    include
)

set(PLATFORM_SPECIFIC_SOURCES_STM32H7xx
    startup_stm32h750xx.s
    Core/Src/system_stm32h7xx.c
    Core/Src/main.c
    Core/Src/dma.c
    Core/Src/freertos.c
    Core/Src/gpio.c
    Core/Src/i2s.c
    Core/Src/spi.c
    Core/Src/usart.c
    Core/Src/stm32h7xx_it.c
    Core/Src/stm32h7xx_hal_msp.c
    Core/Src/stm32h7xx_hal_timebase_tim.c
    USB_HOST/App/usb_host.c
    USB_HOST/Target/usbh_conf.c
    USB_HOST/Target/usbh_platform.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hcd.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_usb.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2s.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2s_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c
    Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.c
    Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.c
    Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.c
    Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.c
    Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Src/usbh_audio.c
    Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Src/usbh_cdc.c
    ../../extern/ILI9341-STM32-HAL/ILI9341/ili9341_font.c
    ../../extern/ILI9341-STM32-HAL/ILI9341/ili9341_gfx.c
    ../../extern/ILI9341-STM32-HAL/ILI9341/ili9341.c
    src/Keys.cpp
)

set(PLATFORM_SPECIFIC_DEFINES_STM32H7xx -DUSE_HAL_DRIVER -DSTM32H750xx
                                        -DSTM32H7
)

set(PLATFORM_SPECIFIC_OPTIONS_STM32H7xx -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16
                                        -mfloat-abi=hard
)

set(PLATFORM_SPECIFIC_LINKER_OPTIONS_STM32H7xx
    -T${CMAKE_CURRENT_LIST_DIR}/STM32H750VBTx_RAM.ld -mcpu=cortex-m7 -mthumb
    -mfpu=fpv5-d16 -mfloat-abi=hard -Wl,-Map=STM32H750.map
)

# ##############################################################################
# Platform Selection
# ##############################################################################

# TODO option to select platform, currently hard-coded to STM32H7xx
set(PLATFORM_SPECIFIC_INCLUDES ${PLATFORM_SPECIFIC_INCLUDES_STM32H7xx})
set(PLATFORM_SPECIFIC_SOURCES ${PLATFORM_SPECIFIC_SOURCES_STM32H7xx})
set(PLATFORM_SPECIFIC_DEFINES ${PLATFORM_SPECIFIC_DEFINES_STM32H7xx})
set(PLATFORM_SPECIFIC_OPTIONS ${PLATFORM_SPECIFIC_OPTIONS_STM32H7xx})
set(PLATFORM_SPECIFIC_LINKER_OPTIONS
    ${PLATFORM_SPECIFIC_LINKER_OPTIONS_STM32H7xx}
)

# ##############################################################################
# Platform-Independent Includes and Sources
# ##############################################################################

set(PLATFORM_INDEPENDENT_SOURCES
    ../../extern/freertos-addons/c++/Source/cmutex.cpp
    ../../extern/freertos-addons/c++/Source/cqueue.cpp
    ../../extern/freertos-addons/c++/Source/csemaphore.cpp
    ../../extern/freertos-addons/c++/Source/cthread.cpp
    ../../extern/fonas/src/fonas.cpp
    ../../extern/fonas/src/EventDrivenReaderWriter.cpp
    ../../extern/fonas/src/EventDrivenWriter.cpp
    ../../extern/fonas/src/EventDrivenSpi.cpp
    ../../extern/fonas/src/StreamBuffer.cpp
    ../../source/stdio.cpp
    ../../source/m8ec.cpp
    ../../source/m8_protocol.cpp
    ../../source/display.cpp
    ../../source/font.cpp
    ../../source/slip.c
    ../../source/periph/UartCallbacks.cpp
    ../../source/periph/Uart4.cpp
    ../../source/periph/SpiCallbacks.cpp
    ../../source/periph/Spi1.cpp
    ../../source/periph/UsbCdc.cpp
    ../../source/KeysThread.cpp
)

set(PLATFORM_INDEPENDENT_INCLUDES
    ../../extern/freertos-addons/c++/Source/include ../../extern/fonas/include
    ../../include
)

set(PLATFORM_INDEPENDENT_DEFINES -DCPP_FREERTOS_NO_EXCEPTIONS
                                 -DCPP_FREERTOS_NO_CPP_STRINGS -DFONAS
)

set(PLATFORM_INDEPENDENT_OPTIONS
    -fstack-usage
    -Wall
    -Wextra
    -fdata-sections
    -ffunction-sections
    # -flto -fno-use-cxa-atexit -fno-threadsafe-statics -fno-rtti
    -fno-exceptions # big change in code size and performace
    # -c -x assembler-with-cpp
    # https://developer.arm.com/documentation/100748/0623/Writing-Optimized-Code/Optimizing-for-code-size-or-performance
    $<$<CONFIG:Debug>:-Og
    -DDEBUG>
    # For good debug experience, Arm recommends using -O1 optimization level
    # rather than -O0. When using -O1, the compiler performs certain
    # optimizations, but the structure of the generated code is still close to
    # the source code.
    $<$<CONFIG:Release>:-O1>
    # -g0 # no debug information -g1 # minimal debug information
    -g # default debug information
    # -g3 # maximal debug information
    -gdwarf-2 # DWARF version 2 debugging information
)

set(PLATFORM_INDEPENDENT_LINKER_OPTIONS
    -fsingle-precision-constant
    -u
    _printf_float # enables floating point in printf
    -Wl,--start-group
    -lc
    -lm
    -lstdc++
    -lsupc++
    -fdata-sections
    -ffunction-sections
    # -flto
    -Wl,--end-group
    --specs=nosys.specs
    -static
    -Wl,--gc-sections
    -Wl,--cref
    -Wl,--print-memory-usage
    -Wl,--no-warn-rwx-segment
)

# ##############################################################################
# Firmware Target
# ##############################################################################

add_executable(
  STM32H750 ${PLATFORM_INDEPENDENT_SOURCES} ${PLATFORM_SPECIFIC_SOURCES}
)

target_compile_definitions(
  STM32H750 PRIVATE ${PLATFORM_INDEPENDENT_DEFINES}
                    ${PLATFORM_SPECIFIC_DEFINES}
)

target_include_directories(
  STM32H750 PRIVATE ${PLATFORM_INDEPENDENT_INCLUDES}
                    ${PLATFORM_SPECIFIC_INCLUDES}
)

target_compile_options(
  STM32H750 PRIVATE ${PLATFORM_INDEPENDENT_OPTIONS}
                    ${PLATFORM_SPECIFIC_OPTIONS}
)

target_link_options(
  STM32H750 PRIVATE ${PLATFORM_INDEPENDENT_LINKER_OPTIONS}
  ${PLATFORM_SPECIFIC_LINKER_OPTIONS}
)

# SEGGER SystemView

add_subdirectory(config/SEGGER_SYSVIEW)

if(SEGGER_SYSVIEW_ENABLED)
  set(PLATFORM_SPECIFIC_OPTIONS
      ${PLATFORM_SPECIFIC_OPTIONS}
      -finstrument-functions
      -finstrument-functions-exclude-file-list=Core/Src/system_stm32h7xx,Core/Src/main,Core/Src/dma,Core/Src/freertos,Core/Src/gpio,Core/Src/i2s,Core/Src/spi,Core/Src/usart,Core/Src/stm32h7xx_it,Core/Src/stm32h7xx_hal_msp,Core/Src/stm32h7xx_hal_timebase_tim,Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hcd,Drivers/CMSIS/Include/cmsis_gcc.h,Drivers/CMSIS/Include/core_cm7.h,Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_usb,Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc,Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex,Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash,Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex,Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio,Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem,Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma,Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex,Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma,Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr,Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex,Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex,Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal,Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti,Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2s,Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2s_ex,Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi,Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi_ex,Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim,Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex,Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart,Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex,extern/SystemView_Src_V352a/SEGGER/SEGGER_RTT_ASM_ARMv7M.S,source/stdio,SYSVIEW/SEGGER_SYSVIEW_FreeRTOS,SYSVIEW/Config/Cortex-M/SeggerSYSVIEW_Config_FreeRTOS,source/periph/UartCallbacks,source/periph/SpiCallbacks,source/KeysThread,extern/SystemView_Src_V352a/SEGGER/SEGGER_RTT,extern/SystemView_Src_V352a/SEGGER/SEGGER_RTT_printf,extern/SystemView_Src_V352a/SEGGER/SEGGER_SYSVIEW,extern/SystemView_Src_V352a/SEGGER/SYSVIEW
  )
endif()


# FreeRTOS

add_library(freertos_config INTERFACE)
target_include_directories(
  freertos_config
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/config
            Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
            Drivers/STM32H7xx_HAL_Driver/Inc # for stm32h7xx_hal.h
            Core/Inc # stm32h7xx_hal_conf.h
)
target_compile_definitions(
  freertos_config INTERFACE -DUSE_HAL_DRIVER -DSTM32H750xx -DSTM32H7
)

target_sources(
  freertos_config
  PUBLIC Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
         Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
)

target_link_libraries(
  freertos_config INTERFACE SeggerSysView ucprof_config ucprof CMSIS
)
add_subdirectory(Middlewares/Third_Party/FreeRTOS)

# CMSIS

add_subdirectory(Drivers/CMSIS)
target_link_libraries(STM32H750 PUBLIC FreeRTOS SeggerSysView ucprof CMSIS)

add_custom_command(
  TARGET STM32H750
  POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O ihex STM32H750 STM32H750.hex
  COMMAND ${CMAKE_OBJCOPY} -O binary STM32H750 STM32H750.bin
  COMMAND ${CMAKE_OBJDUMP} -S -t STM32H750 >
          ${CMAKE_CURRENT_BINARY_DIR}/STM32H750.dump
  COMMAND ${CMAKE_NM} STM32H750 -C -n -S -s > STM32H750.address-sort.nm
  COMMAND ${CMAKE_NM} STM32H750 -C -S -s --size-sort > STM32H750.size-sort.nm
  COMMAND arm-none-eabi-size STM32H750
)
