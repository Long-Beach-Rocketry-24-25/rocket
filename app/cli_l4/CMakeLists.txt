set(EXECUTABLE cli_l4)
set(LDF ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE}.ld)

set(PREPROCESS_DEFS
    -DDEF_FLASH_START_ADDR=0x8000000
    -DDEF_FLASH_SIZE=1024K
)

add_subdirectory_for(STM32L4 BSP)

add_executable_for(STM32L4 ${EXECUTABLE} ${LDF}
    main.c
    freertos.c
    interrupts.c
    ${STARTUP_FILE}
)

target_link_libraries_for(STM32L4 ${EXECUTABLE} PRIVATE
    bsp
)

target_preprocess_for(STM32L4 ${EXECUTABLE} ${LINKER_SCRIPT} ${LDF} ${PREPROCESS_DEFS})
