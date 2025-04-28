set(EXECUTABLE nidec_24h_test)
set(LDF ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE}.ld)

set(PREPROCESS_DEFS
    -DDEF_FLASH_START_ADDR=0x8000000
    -DDEF_FLASH_SIZE=1024K
)

add_subdirectory_for(STM32L412 BSP_L412)
add_subdirectory_for(STM32L476 BSP_L476)

if (TARGET nidec_bsp)

    add_executable_for(${TARGET_DEVICE} ${EXECUTABLE} ${LDF}
        main.c
        freertos.c
        interrupts.c
        ${STARTUP_FILE}
    )

    target_link_libraries_for(${TARGET_DEVICE} ${EXECUTABLE} PRIVATE
        nidec_bsp
    )

    target_preprocess_for(${TARGET_DEVICE} ${EXECUTABLE} ${LINKER_SCRIPT} ${LDF} ${PREPROCESS_DEFS})

endif()