add_library(hal STATIC
    st_gpio.c
    st_i2c.c
    st_usart.c
)

target_include_directories(hal PUBLIC
    bus
    .
)
target_link_libraries(hal PUBLIC
    core
    st_l4_support
)
