set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

if(MINGW
   OR CYGWIN
   OR WIN32
)
  set(UTIL_SEARCH_CMD where)
elseif(UNIX OR APPLE)
  set(UTIL_SEARCH_CMD which)
endif()

set(TOOLCHAIN_PREFIX "arm-none-eabi-")

execute_process(
  COMMAND ${UTIL_SEARCH_CMD} ${TOOLCHAIN_PREFIX}gcc
  OUTPUT_VARIABLE BINUTILS_PATH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

get_filename_component(ARM_TOOLCHAIN_DIR ${BINUTILS_PATH} DIRECTORY)
# set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY) # causes problems on WSL env
# setup

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)

set(CMAKE_AR ${TOOLCHAIN_PREFIX}ar)

set(CMAKE_OBJCOPY
    ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}objcopy
    CACHE INTERNAL "objcopy tool"
)
set(CMAKE_OBJDUMP
    ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}objdump
    CACHE INTERNAL "objdump tool"
)
set(CMAKE_NM
    ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}nm
    CACHE INTERNAL "nm tool"
)
set(CMAKE_SIZE_UTIL
    ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}size
    CACHE INTERNAL "size tool"
)

set(CMAKE_FIND_ROOT_PATH ${BINUTILS_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

set(PLATFORM_STM32H750_FLAGS
    "-mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard"
)

set(COMMON_FLAGS "${PLATFORM_STM32H750_FLAGS}" # -flto
)

set(COMPILER_FLAGS "-fdata-sections -ffunction-sections")

set(CMAKE_C_FLAGS "${COMMON_FLAGS} ${COMPILER_FLAGS}")

set(CMAKE_CXX_FLAGS
    "${COMMON_FLAGS} ${COMPILER_FLAGS} -fno-rtti -fno-exceptions -fno-threadsafe-statics"
)
set(CMAKE_ASM_FLAGS "-x assembler-with-cpp ${COMMON_FLAGS} ${COMPILER_FLAGS}")

set(CMAKE_EXE_LINKER_FLAGS
    "${COMMON_FLAGS} --specs=nosys.specs -static -Wl,--gc-sections -Wl,--print-memory-usage -u _printf_float -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group"
)
