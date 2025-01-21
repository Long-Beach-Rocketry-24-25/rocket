add_library(cli_app_bsp STATIC
    cli_app_bsp.c
    hal_timebase.c
)

target_include_directories(cli_app_bsp PUBLIC .)
target_link_libraries(cli_app_bsp
    PUBLIC
        app
        core
        driver
        st_l4_support
    INTERFACE
        freertos_kernel
)
