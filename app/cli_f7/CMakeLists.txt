set(EXECUTABLE cli_f7)

add_subdirectory(BSP)

add_executable(${EXECUTABLE}
    main.c
    freertos.c
    interrupts.c
    usart_cli.c
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
    cli
    drivers
    timeout
    container
)

add_custom_command(TARGET ${EXECUTABLE}
    POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${EXECUTABLE}.hex
    COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${EXECUTABLE}.bin
    COMMAND arm-none-eabi-objcopy -O srec ${EXECUTABLE} ${EXECUTABLE}.elf
)
