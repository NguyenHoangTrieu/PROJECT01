set(STM32CUBEMX_GENERATED_FILES
        ${CMAKE_CURRENT_LIST_DIR}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
        ${CMAKE_CURRENT_LIST_DIR}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
        ${CMAKE_CURRENT_LIST_DIR}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
        ${CMAKE_CURRENT_LIST_DIR}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
        ${CMAKE_CURRENT_LIST_DIR}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
        ${CMAKE_CURRENT_LIST_DIR}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
        ${CMAKE_CURRENT_LIST_DIR}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
        ${CMAKE_CURRENT_LIST_DIR}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
        ${CMAKE_CURRENT_LIST_DIR}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
        ${CMAKE_CURRENT_LIST_DIR}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
        ${CMAKE_CURRENT_LIST_DIR}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
        ${CMAKE_CURRENT_LIST_DIR}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
        ${CMAKE_CURRENT_LIST_DIR}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
        ${CMAKE_CURRENT_LIST_DIR}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
        ${CMAKE_CURRENT_LIST_DIR}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
        ${CMAKE_CURRENT_LIST_DIR}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
        ${CMAKE_CURRENT_LIST_DIR}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c
        ${CMAKE_CURRENT_LIST_DIR}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c
        ${CMAKE_CURRENT_LIST_DIR}/../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_adc.c

        ${CMAKE_CURRENT_LIST_DIR}/../Inc/main.h
        ${CMAKE_CURRENT_LIST_DIR}/../Inc/stm32f4xx_hal_conf.h
        ${CMAKE_CURRENT_LIST_DIR}/../Inc/stm32f4xx_it.h
        ${CMAKE_CURRENT_LIST_DIR}/../Inc/FreeRTOSConfig.h

        ${CMAKE_CURRENT_LIST_DIR}/../Src/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../Src/stm32f4xx_hal_msp.c
        ${CMAKE_CURRENT_LIST_DIR}/../Src/stm32f4xx_it.c
        ${CMAKE_CURRENT_LIST_DIR}/../Src/system_stm32f4xx.c
        ${CMAKE_CURRENT_LIST_DIR}/../Src/stm32f4xx_hal_timebase_tim.c
        ${CMAKE_CURRENT_LIST_DIR}/../Src/sysmem.c
        
        ${CMAKE_CURRENT_LIST_DIR}/../startup_stm32f411xe.s)

set(MIDDLEWARES_ADDED
        ${CMAKE_CURRENT_LIST_DIR}/../Third_Party/FreeRTOS/Source/croutine.c 
        ${CMAKE_CURRENT_LIST_DIR}/../Third_Party/FreeRTOS/Source/event_groups.c 
        ${CMAKE_CURRENT_LIST_DIR}/../Third_Party/FreeRTOS/Source/list.c 
        ${CMAKE_CURRENT_LIST_DIR}/../Third_Party/FreeRTOS/Source/queue.c 
        ${CMAKE_CURRENT_LIST_DIR}/../Third_Party/FreeRTOS/Source/stream_buffer.c 
        ${CMAKE_CURRENT_LIST_DIR}/../Third_Party/FreeRTOS/Source/tasks.c 
        ${CMAKE_CURRENT_LIST_DIR}/../Third_Party/FreeRTOS/Source/timers.c 
        ${CMAKE_CURRENT_LIST_DIR}/../Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c 
        ${CMAKE_CURRENT_LIST_DIR}/../Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
        ${CMAKE_CURRENT_LIST_DIR}/../Third_Party/FreeRTOS/Source/include/FreeRTOS.h

        ${CMAKE_CURRENT_LIST_DIR}/../LoRa_AS32_lib/Inc/LoRa_AS32_lib.h
        ${CMAKE_CURRENT_LIST_DIR}/../LoRa_AS32_lib/Src/LoRa_AS32_lib.c

        ${CMAKE_CURRENT_LIST_DIR}/../HEAT_Routing/Src/MessageReadWrite.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../HEAT_Routing/Src/MessageParser.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../HEAT_Routing/Src/task_handler.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../HEAT_Routing/Src/peripheral.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../HEAT_Routing/Src/task_test_handler.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../HEAT_Routing/Src/Message_Task_Handler.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../HEAT_Routing/Src/TimerCallback.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../HEAT_Routing/Src/Calculate_Function.cpp

        ${CMAKE_CURRENT_LIST_DIR}/../HEAT_Routing/Inc/FunctionHeader/MessageReadWrite.hpp
        ${CMAKE_CURRENT_LIST_DIR}/../HEAT_Routing/Inc/FunctionHeader/protocolNaming.hpp
        ${CMAKE_CURRENT_LIST_DIR}/../HEAT_Routing/Inc/MessageParser.hpp
        ${CMAKE_CURRENT_LIST_DIR}/../HEAT_Routing/Inc/task_handler.hpp
        ${CMAKE_CURRENT_LIST_DIR}/../HEAT_Routing/Inc/peripheral.hpp
)

set(GLOBAL_INCLUDE_DIRS
        ${CMAKE_CURRENT_LIST_DIR}/../Inc
        ${CMAKE_CURRENT_LIST_DIR}/../LoRa_AS32_lib/Inc
        ${CMAKE_CURRENT_LIST_DIR}/../Drivers/STM32F4xx_HAL_Driver/Inc
        ${CMAKE_CURRENT_LIST_DIR}/../Drivers/CMSIS/Device/ST/STM32F4xx/Include
        ${CMAKE_CURRENT_LIST_DIR}/../Drivers/CMSIS/Include
        ${CMAKE_CURRENT_LIST_DIR}/../Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
        ${CMAKE_CURRENT_LIST_DIR}/../Third_Party/FreeRTOS/Source/include
        ${CMAKE_CURRENT_LIST_DIR}/../HEAT_Routing/Inc/FunctionHeader
        ${CMAKE_CURRENT_LIST_DIR}/../HEAT_Routing/Inc)