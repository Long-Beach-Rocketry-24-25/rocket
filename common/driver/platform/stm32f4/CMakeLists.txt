add_library(hal STATIC
    st_gpio.c
)

target_include_directories(hal PUBLIC
    bus
    io
    .
)
target_link_libraries(hal PUBLIC
    core
    st_f4_support
)
