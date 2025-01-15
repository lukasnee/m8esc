if(SEGGER_SYSVIEW_ENABLED)
  add_library(SeggerSysView_config_STM32H750 STATIC)
  target_sources(
    SeggerSysView_config_STM32H750 PRIVATE SEGGER_SYSVIEW_Config_FreeRTOS.c
                                           SEGGER_SYSVIEW_FreeRTOS.c
  )
  target_include_directories(
    SeggerSysView_config_STM32H750 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
  )
  target_link_libraries(SeggerSysView_config_STM32H750 PRIVATE freertos_kernel)

  target_compile_options(
    SeggerSysView_config_STM32H750
    PRIVATE
      -finstrument-functions
      -finstrument-functions-exclude-file-list=SEGGER_SYSVIEW_FreeRTOS,SEGGER_SYSVIEW_Config_FreeRTOS
  )

else()
  add_library(SeggerSysView_config_STM32H750 INTERFACE)
  # target_include_directories(SeggerSysView_config_STM32H750 INTERFACE
  # ${CMAKE_CURRENT_SOURCE_DIR})
  # target_link_libraries(SeggerSysView_config_STM32H750 INTERFACE freertos_kernel)
endif()
