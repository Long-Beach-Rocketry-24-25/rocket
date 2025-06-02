set(EXECUTABLE subscale)
set(LDF ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE}.ld)

set(PREPROCESS_DEFS
    -DDEF_FLASH_START_ADDR=0x8000000
    -DDEF_FLASH_SIZE=1024K
)

add_subdirectory_for(STM32F746 BSP_F746)
add_subdirectory_for(STM32H503 BSP_H503)
add_subdirectory_for(STM32L476 BSP_L476)

if (TARGET subscale_bsp)
    if(MCU_NAME MATCHES "^(STM32L4)[0-9]+") 
        add_executable_for(${TARGET_DEVICE} ${EXECUTABLE} ${LDF}
        main_rayne.c
        freertos.c
        interrupts.c
        ${STARTUP_FILE}
        )

        target_link_libraries_for(${TARGET_DEVICE} ${EXECUTABLE} PRIVATE
            subscale_bsp
        ) 
    else()
    add_executable_for(${TARGET_DEVICE} ${EXECUTABLE} ${LDF}
        main.c
        freertos.c
        interrupts.c
        ${STARTUP_FILE}
    )

    target_link_libraries_for(${TARGET_DEVICE} ${EXECUTABLE} PRIVATE
        subscale_bsp
    )

    target_preprocess_for(${TARGET_DEVICE} ${EXECUTABLE} ${LINKER_SCRIPT} ${LDF} ${PREPROCESS_DEFS})
    
    endif()

endif()