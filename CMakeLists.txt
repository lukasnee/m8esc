cmake_minimum_required(VERSION 3.16)

project(m8ec)

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

enable_language(CXX C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

if(PLATFORM STREQUAL "STM32H750")

elseif(PLATFORM STREQUAL "STM32F411")

else()
  if(NOT PLATFORM)
    message(FATAL_ERROR "PLATFORM not set")
  else()
    message(FATAL_ERROR "Unknown PLATFORM: ${PLATFORM}")
  endif()
endif()

add_library(SeggerSysView_config INTERFACE)
target_link_libraries(
  SeggerSysView_config INTERFACE SeggerSysView_config_STM32H750
)

add_library(ucprof_config INTERFACE)
target_include_directories(
  ucprof_config INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/config
)

add_subdirectory(extern/ucprof)

add_subdirectory(platform/${PLATFORM})
