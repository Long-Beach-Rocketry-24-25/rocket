add_library(cli_app_bsp STATIC
    cli_app_bsp.c
)

target_include_directories(cli_app_bsp PUBLIC .)
target_link_libraries(cli_app_bsp
    PUBLIC
        app
        core
        driver
        st_h5_support
    INTERFACE
        freertos_kernel
)