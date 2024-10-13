
add_library(cli STATIC
    cli.c
)

target_include_directories(cli PUBLIC .)

add_subdirectory(usart)
add_subdirectory(i2c_access)
add_subdirectory(blink)
add_subdirectory(bmp390)
add_subdirectory(bno055)
add_subdirectory(tmp102)