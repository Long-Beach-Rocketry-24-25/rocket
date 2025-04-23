add_library(nidec_bsp STATIC
    nidec_bsp.c
)

target_include_directories(nidec_bsp PUBLIC .)
target_link_libraries(nidec_bsp
    PUBLIC
        app
        core
        driver
    INTERFACE
        freertos_kernel
)
