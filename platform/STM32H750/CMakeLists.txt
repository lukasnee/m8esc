project(platform)

add_library(platform INTERFACE)

target_sources(
  platform
  INTERFACE startup_stm32h750xx.s
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
            Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.c
            Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.c
            Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.c
            Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.c
            Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Src/usbh_audio.c
            Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Src/usbh_cdc.c
            Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
)

target_compile_definitions(
  platform
  INTERFACE # platform independent
            -DCPP_FREERTOS_NO_EXCEPTIONS
            -DCPP_FREERTOS_NO_CPP_STRINGS
            -DFONAS
            # platform-specific
            -DUSE_HAL_DRIVER
            -DSTM32H750xx
            -DSTM32H7
)

target_include_directories(
  platform
  INTERFACE # platform-independent:
            ../../extern/freertos-addons/c++/Source/include
            ../../extern/fonas/include
            ../../include
            # platform-specific
            Core/Inc
            ../../extern/ILI9341-STM32-HAL
            include
            Middlewares/Third_Party/FreeRTOS/Source/include
            Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
)

target_include_directories(
  platform
  INTERFACE USB_HOST/App
            USB_HOST/Target
            Middlewares/ST/STM32_USB_Host_Library/Core/Inc
            Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc
            Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Inc
)

if(SEGGER_SYSVIEW_ENABLED)
  target_compile_options(
    platform
    PRIVATE
      -finstrument-functions
      -finstrument-functions-exclude-file-list=Core/Src/system_stm32h7xx,Core/Src/main,Core/Src/dma,Core/Src/freertos,Core/Src/gpio,Core/Src/i2s,Core/Src/spi,Core/Src/usart,Core/Src/stm32h7xx_it,Core/Src/stm32h7xx_hal_msp,Core/Src/stm32h7xx_hal_timebase_tim
  )
endif()

target_link_options(
  platform
  INTERFACE
  -T${CMAKE_CURRENT_LIST_DIR}/STM32H750VBTx_RAM.ld
  -Wl,--no-warn-rwx-segment
#   -Wl,--undefined=uxTopUsedPriority
)

add_subdirectory(Drivers/CMSIS)

add_library(STM32H7xx_HAL_conf INTERFACE)
target_include_directories(
  STM32H7xx_HAL_conf
  INTERFACE Core/Inc Drivers/CMSIS/Device/ST/STM32H7xx/Include
) # Core/Inc/stm32h7xx_hal_conf.h
add_subdirectory(Drivers/STM32H7xx_HAL_Driver)

add_subdirectory(config/SEGGER_SYSVIEW)

target_link_libraries(
  platform
  INTERFACE STM32H7xx_HAL
            freertos_kernel
            SeggerSysView
            ucprof
            CMSIS
)
