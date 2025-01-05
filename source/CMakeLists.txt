add_executable(m8ec)

target_sources(
  m8ec
  PRIVATE stdio.cpp
          m8ec.cpp
          m8_protocol.cpp
          display.cpp
          font.cpp
          slip.c
          periph/UartCallbacks.cpp
          periph/Uart4.cpp
          periph/SpiCallbacks.cpp
          periph/Spi1.cpp
          periph/UsbCdc.cpp
          KeysThread.cpp
          platform/${PLATFORM}/Keys.cpp
)

target_include_directories(m8ec PUBLIC ../include)

target_link_options(m8ec PUBLIC -Wl,-Map=m8ec.map -Wl,--cref)

target_link_libraries(
  m8ec PUBLIC platform SeggerSysView fonas ILI9341-STM32-HAL
)

add_custom_command(
  TARGET m8ec
  POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O ihex m8ec m8ec.hex
  COMMAND ${CMAKE_OBJCOPY} -O binary m8ec m8ec.bin
  COMMAND ${CMAKE_OBJDUMP} -S -t m8ec > ${CMAKE_CURRENT_BINARY_DIR}/m8ec.dump
  COMMAND ${CMAKE_NM} m8ec -C -n -S -s > m8ec.address-sort.nm
  COMMAND ${CMAKE_NM} m8ec -C -S -s --size-sort > m8ec.size-sort.nm
  COMMAND arm-none-eabi-size m8ec
)

if(SEGGER_SYSVIEW_ENABLED)
  target_compile_options(
    m8ec
    PRIVATE
      -finstrument-functions
      -finstrument-functions-exclude-file-list=stdio,periph/UartCallbacks,periph/SpiCallbacks,KeysThread
  )
endif()
