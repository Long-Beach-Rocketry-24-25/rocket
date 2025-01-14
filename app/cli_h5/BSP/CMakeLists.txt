add_library(freertos_config INTERFACE)
target_include_directories(freertos_config SYSTEM INTERFACE .) # The config file directory
target_compile_definitions(freertos_config INTERFACE 
    projCOVERAGE_TEST=0
    ${definitions}
)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/FreeRTOS-Kernel ${CMAKE_SOURCE_DIR}/build/FreeRTOS)
add_subdirectory(${CMAKE_SOURCE_DIR}/common/app/freertos ${CMAKE_SOURCE_DIR}/build/freertos_app)

add_library(bsp STATIC
    bsp.c
)

target_include_directories(bsp PUBLIC .)
target_link_libraries(bsp
    PUBLIC
        freertos_app
        core
        driver
        st_h5_support
    INTERFACE
        freertos_kernel
)