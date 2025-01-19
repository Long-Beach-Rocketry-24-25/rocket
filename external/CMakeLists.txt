
if (NOT "${FREERTOS_PORT}" STREQUAL "")
    add_library(freertos_config INTERFACE)
    # The config file directory
    target_compile_definitions(freertos_config INTERFACE 
        projCOVERAGE_TEST=0
    )
    target_link_libraries(freertos_config INTERFACE hal)

    add_subdirectory(FreeRTOS-Kernel)
endif()
