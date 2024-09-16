add_library(gpio STATIC
    st_gpio.c
)

target_include_directories(gpio PUBLIC .)
