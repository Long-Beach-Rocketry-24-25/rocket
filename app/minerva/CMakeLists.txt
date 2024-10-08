set(EXECUTABLE minerva)

add_subdirectory(BSP)

add_executable(${EXECUTABLE}
    main.c
    freertos.c
    interrupts.c
    ${STARTUP_FILE}
)

target_link_options(${EXECUTABLE} PRIVATE
    -T${LINKER_SCRIPT}
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
    usart
    i2c
    tmp102
    ads111x
    gnc
)

add_custom_command(TARGET ${EXECUTABLE}
    POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${EXECUTABLE}.hex
    COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${EXECUTABLE}.bin
    COMMAND arm-none-eabi-objcopy -O srec ${EXECUTABLE} ${EXECUTABLE}.elf
)
