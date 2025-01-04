add_library(FreeRTOS STATIC)
target_include_directories(FreeRTOS PUBLIC Source/include Source/CMSIS_RTOS_V2)

target_sources(
  FreeRTOS
  PRIVATE Source/croutine.c
          Source/event_groups.c
          Source/list.c
          Source/queue.c
          Source/stream_buffer.c
          Source/tasks.c
          # Source/timers.c
          Source/CMSIS_RTOS_V2/cmsis_os2.c
)

if(NOT TARGET freertos_config)
  message(FATAL_ERROR "freertos_config not found")
endif()
target_link_libraries(FreeRTOS PUBLIC freertos_config)

if(SEGGER_SYSVIEW_ENABLED)
  target_compile_options(
    FreeRTOS
    PRIVATE
      -finstrument-functions
      -finstrument-functions-exclude-file-list=Source/croutine,Source/event_groups,Source/list,Source/queue,Source/stream_buffer,Source/tasks,Source/CMSIS_RTOS_V2/cmsis_os2
  )
endif()
