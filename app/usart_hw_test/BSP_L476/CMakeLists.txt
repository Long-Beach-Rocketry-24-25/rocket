add_library(usart_hw_bsp STATIC
    usart_hw_bsp.c
    hal_timebase.c
)

target_include_directories(usart_hw_bsp PUBLIC .)
target_link_libraries(usart_hw_bsp
    PUBLIC
        app
        core
        driver
        st_l4_support
    INTERFACE
        freertos_kernel
)
