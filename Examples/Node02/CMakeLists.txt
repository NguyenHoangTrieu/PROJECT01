cmake_minimum_required(VERSION 3.20)

project(Node_02)

enable_language(C CXX ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../CMakeFile")
message(STATUS "CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")
include(${CMAKE_MODULE_PATH}/Flag.cmake)
include(${CMAKE_MODULE_PATH}/CommonPath.cmake)

set(EXECUTABLE ${PROJECT_NAME}.out)

add_executable(${EXECUTABLE}
        HeatConfig.h
        ${STM32CUBEMX_GENERATED_FILES}
        ${MIDDLEWARES_ADDED})

target_compile_definitions(${EXECUTABLE} PRIVATE
        -DUSE_HAL_DRIVER
        -DSTM32F413xx
)

target_include_directories(${EXECUTABLE} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUDE_DIRS}
)

target_compile_options(${EXECUTABLE} PRIVATE
        ${COMPILER_FLAGS}
        ${DEBUG_FLAGs}
        ${CXX_FLAGS}
)

target_link_options(${EXECUTABLE} PRIVATE
        ${LINKER_FLAGS}
)

add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND arm-none-eabi-size ${EXECUTABLE})

add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
        COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin)

set_source_files_properties(
        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
        PROPERTIES
        COMPILE_FLAGS -Wno-unused-parameter)
