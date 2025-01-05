add_library(
  m8ec INTERFACE
)

target_sources(m8ec INTERFACE 
stdio.cpp
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

target_include_directories(m8ec INTERFACE ../include)

target_link_libraries(
  m8ec INTERFACE platform SeggerSysView fonas ILI9341-STM32-HAL
)

if(SEGGER_SYSVIEW_ENABLED)
  target_compile_options(
    m8ec
    INTERFACE
      -finstrument-functions
      -finstrument-functions-exclude-file-list=stdio,periph/UartCallbacks,periph/SpiCallbacks,KeysThread
  )
endif()
