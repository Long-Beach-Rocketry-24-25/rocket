
add_library(periph STATIC
    ads111x/ads111x.c
    bmp390/bmp390.c
    bno055/bno055.c
    chip_select/gpio_cs.c
    tmp102/tmp102.c
    w25q/w25q.c
    w25q/mock_w25q.c
)

target_include_directories(periph PUBLIC
    ads111x
    bmp390
    bno055
    chip_select
    tmp102
    w25q
)

target_link_libraries(periph PUBLIC driver)