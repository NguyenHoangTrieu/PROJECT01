set(COMPILER_FLAGS
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard

        -fdata-sections
        -ffunction-sections

        -Wall
        #-Wextra
        #-pedantic
)

set(DEBUG_FLAGS
        $<$<CONFIG:Debug>:-Og>
)

set(CXX_FLAGS
        $<$<COMPILE_LANGUAGE:CXX>:
        -fno-exceptions
        -fno-rtti
        #-Wold-style-cast
        #-Wuseless-cast
        #-Wsuggest-override
        >
)

set(LINKER_FLAGS
        -T${CMAKE_CURRENT_LIST_DIR}/../stm32f411vetx_flash.ld
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard

        -specs=nano.specs
        -lc
        -lm
        -lnosys

        -Wl,-Map=${PROJECT_NAME}.map,--cref
        -Wl,--gc-sections
)
