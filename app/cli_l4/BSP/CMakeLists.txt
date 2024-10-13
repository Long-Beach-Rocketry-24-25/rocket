add_library(freertos_config INTERFACE)
target_include_directories(freertos_config SYSTEM INTERFACE .) # The config file directory
target_compile_definitions(freertos_config INTERFACE 
    projCOVERAGE_TEST=0
    ${definitions}
)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/FreeRTOS-Kernel ../Build/l452)

add_library(bsp STATIC
    bsp.c
    hal_timebase.c
)

target_include_directories(bsp PUBLIC .)
target_link_libraries(bsp
    PUBLIC
        st_l4_support
        usart_cli
        usart
        i2c
        gpio
        timeout
    INTERFACE
        freertos_kernel
)
