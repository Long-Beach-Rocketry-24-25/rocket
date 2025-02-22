add_library(pwm_app_bsp STATIC
    pwm_app_bsp.c
)

target_include_directories(pwm_app_bsp PUBLIC .)
target_link_libraries(pwm_app_bsp
    PUBLIC
        app
        core
        driver
        st_l4_support
    INTERFACE
        freertos_kernel
)
