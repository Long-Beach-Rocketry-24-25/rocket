add_library(subscale_bsp STATIC
    subscale_bsp.c
)

target_include_directories(subscale_bsp PUBLIC .)
target_link_libraries(subscale_bsp
    PUBLIC
        app
        core
        driver
        st_f7_support
    INTERFACE
        freertos_kernel
)