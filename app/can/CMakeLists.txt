set(MCU_SUPPORT ${CMAKE_SOURCE_DIR}/mcu_support/stm32/h5xx/h503)
set(LINKER_SCRIPT ${MCU_SUPPORT}/STM32H503xx_FLASH.ld)
set(STARTUP_FILE ${MCU_SUPPORT}/startup_stm32h503xx.s)

set(EXECUTABLE can)

add_subdirectory(BSP)

add_executable(${EXECUTABLE}
    main.c
    freertos.c
    interrupts.c
    ${STARTUP_FILE}
)

target_link_options(${EXECUTABLE} PUBLIC
    -T${LINKER_SCRIPT}
    -mcpu=cortex-m33
    -mthumb
    -mfpu=fpv5-sp-d16
    -mfloat-abi=hard
    -specs=nosys.specs
    -lc
    -lm
    -lnosys
    -Wl,-Map=${EXECUTABLE}.map,--cref
    -Wl,--gc-sections
    # -Wl, --print-memory-usage
)

target_link_libraries(${EXECUTABLE} PRIVATE
    bsp
)

add_custom_command(TARGET ${EXECUTABLE}
    POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${EXECUTABLE}.hex
    COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${EXECUTABLE}.bin
    COMMAND arm-none-eabi-objcopy -O srec ${EXECUTABLE} ${EXECUTABLE}.elf
)
