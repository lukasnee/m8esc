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

target_link_libraries(FreeRTOS PUBLIC freertos_config)
