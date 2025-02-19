add_library(tjbus STATIC
    custom_bus.c
)

target_include_directories(tjbus PUBLIC .)

add_tests(tjbus
    format_test
)