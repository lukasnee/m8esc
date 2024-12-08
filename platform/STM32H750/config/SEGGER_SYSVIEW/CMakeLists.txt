
if(SEGGER_SYSVIEW_ENABLED)
add_library(SeggerSysView_config_STM32H750 STATIC)
target_sources(
  SeggerSysView_config_STM32H750 PRIVATE SEGGER_SYSVIEW_Config_FreeRTOS.c
                                   SEGGER_SYSVIEW_FreeRTOS.c
)
target_include_directories(
  SeggerSysView_config_STM32H750 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(SeggerSysView_config_STM32H750 PRIVATE FreeRTOS)
else()
add_library(SeggerSysView_config_STM32H750 INTERFACE)
# target_include_directories(SeggerSysView_config_STM32H750 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
# target_link_libraries(SeggerSysView_config_STM32H750 INTERFACE FreeRTOS)
endif()