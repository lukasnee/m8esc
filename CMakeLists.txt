cmake_minimum_required(VERSION 3.16)

project(m8ec)

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

enable_language(CXX C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(
  -fstack-usage
  -Wall
  -Wextra
  -fno-exceptions # big change in code size and performace
  # https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
  # https://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html
  $<$<CONFIG:Debug>:-g>
  $<$<CONFIG:Debug>:-gdwarf-2>
  $<$<CONFIG:Debug>:-Og>
  $<$<CONFIG:Release>:-O1>
)

add_compile_definitions($<$<CONFIG:Debug>:DEBUG>)

add_library(SeggerSysView_config INTERFACE)
target_link_libraries(
  SeggerSysView_config INTERFACE SeggerSysView_config_STM32H750
)

add_library(ucprof_config INTERFACE)
target_include_directories(
  ucprof_config INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/config
)

add_subdirectory(platform/${PLATFORM})
add_subdirectory(extern/ucprof)

# target_link_libraries( freertos_config INTERFACE STM32H7xx_HAL SeggerSysView
# ucprof_config ucprof CMSIS )
add_library(freertos_config INTERFACE)
target_include_directories(
  freertos_config SYSTEM INTERFACE platform/${PLATFORM}/config
) # FreeRTOSConfig.h
# target_compile_definitions(freertos_config PUBLIC projCOVERAGE_TEST=0)

if(SEGGER_SYSVIEW_ENABLED)
  target_compile_options(
    freertos_config
    INTERFACE
      -finstrument-functions
      -finstrument-functions-exclude-file-list=Source/croutine,Source/event_groups,Source/list,Source/queue,Source/stream_buffer,Source/tasks,Source/CMSIS_RTOS_V2/cmsis_os2
  )
endif()

set(FREERTOS_PORT GCC_ARM_CM7)
set(FREERTOS_HEAP 4)
add_subdirectory(extern/FreeRTOS-Kernel)

add_subdirectory(extern/freertos-addons)
add_subdirectory(extern/fonas)
add_subdirectory(extern/ILI9341-STM32-HAL)

add_subdirectory(source)
