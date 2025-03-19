
add_library(periph STATIC
    count_debounce.c
    ads111x/ads111x.c
    bmp390/bmp390.c
    bno055/bno055.c
    chip_select/gpio_cs.c
    rs485/snx5176b.c
    tmp102/tmp102.c
    w25q/w25q.c
    w25q/mock_w25q.c
)

target_include_directories(periph PUBLIC
    switches
    ads111x
    bmp390
    bno055
    chip_select
    rs485
    tmp102
    w25q
)

target_link_libraries(periph PUBLIC driver)