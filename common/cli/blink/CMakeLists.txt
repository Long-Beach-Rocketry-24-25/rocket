
add_library(blink STATIC
    blink.c
)

target_include_directories(blink PUBLIC .)
target_link_libraries(blink PUBLIC
    cli
    gpio
)