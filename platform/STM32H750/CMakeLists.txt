add_library(
  platform STATIC
  Core/Src/dma.c
  Core/Src/freertos.c
  Core/Src/gpio.c
  Core/Src/i2s.c
  Core/Src/spi.c
  Core/Src/usart.c
  USB_HOST/App/usb_host.c
  USB_HOST/Target/usbh_conf.c
  USB_HOST/Target/usbh_platform.c
  Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.c
  Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.c
  Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.c
  Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.c
  Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Src/usbh_audio.c
  Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Src/usbh_cdc.c
)

target_compile_definitions(
  platform
  PUBLIC # platform independent
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
  PUBLIC # platform-independent:
         ../../extern/freertos-addons/c++/Source/include
         ../../extern/fonas/include
         ../../include
         # platform-specific
         Core/Inc
         ../../extern/ILI9341-STM32-HAL
         include
)

target_include_directories(
  platform
  PUBLIC USB_HOST/App
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

add_subdirectory(Drivers/CMSIS)

add_library(STM32H7xx_HAL_conf INTERFACE)
target_include_directories(
  STM32H7xx_HAL_conf INTERFACE Core/Inc
                               Drivers/CMSIS/Device/ST/STM32H7xx/Include
) # Core/Inc/stm32h7xx_hal_conf.h
add_subdirectory(Drivers/STM32H7xx_HAL_Driver)

add_subdirectory(config/SEGGER_SYSVIEW)

add_library(freertos_config INTERFACE)
target_include_directories(
  freertos_config
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/config
            Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
)
target_link_libraries(freertos_config INTERFACE STM32H7xx_HAL_conf)

target_sources(
  freertos_config
  PUBLIC Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
         Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
)

target_link_libraries(
  freertos_config
  INTERFACE STM32H7xx_HAL
            SeggerSysView
            ucprof_config
            ucprof
            CMSIS
)
add_subdirectory(Middlewares/Third_Party/FreeRTOS)

target_link_libraries(
  platform
  PUBLIC STM32H7xx_HAL
         FreeRTOS
         SeggerSysView
         ucprof
         CMSIS
)

set(PLATFORM_LIST_DIR
    ${CMAKE_CURRENT_LIST_DIR}
    PARENT_SCOPE
)

function(platform_link_app platform_app)
  if(NOT TARGET ${platform_app})
    message(FATAL_ERROR "${platform_app} target not found")
  endif()
  message(STATUS "platform_link_app: ${platform_app}")

  add_executable(
    ${platform_app}_exe
    ${PLATFORM_LIST_DIR}/Core/Src/stm32h7xx_it.c
    ${PLATFORM_LIST_DIR}/startup_stm32h750xx.s
    ${PLATFORM_LIST_DIR}/Core/Src/system_stm32h7xx.c
    ${PLATFORM_LIST_DIR}/Core/Src/main.c
    # ${PLATFORM_LIST_DIR}/Core/Src/syscalls.c
    ${PLATFORM_LIST_DIR}/Core/Src/stm32h7xx_hal_msp.c
    ${PLATFORM_LIST_DIR}/Core/Src/stm32h7xx_hal_timebase_tim.c
  )
  target_link_libraries(${platform_app}_exe PRIVATE platform ${platform_app})

  target_link_options(
    ${platform_app}_exe
    PUBLIC
    -T${PLATFORM_LIST_DIR}/STM32H750VBTx_RAM.ld
    -Wl,-Map=${platform_app}_exe.map
    -Wl,--cref
    -Wl,--no-warn-rwx-segment
  )

  add_custom_command(
    TARGET ${platform_app}_exe
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${platform_app}_exe ${platform_app}_exe.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary ${platform_app}_exe
            ${platform_app}_exe.bin
    COMMAND ${CMAKE_OBJDUMP} -S -t ${platform_app}_exe >
            ${CMAKE_CURRENT_BINARY_DIR}/${platform_app}_exe.dump
    COMMAND ${CMAKE_NM} ${platform_app}_exe -C -n -S -s >
            ${platform_app}_exe.address-sort.nm
    COMMAND ${CMAKE_NM} ${platform_app}_exe -C -S -s --size-sort >
            ${platform_app}_exe.size-sort.nm
    COMMAND arm-none-eabi-size ${platform_app}_exe
  )
endfunction()
