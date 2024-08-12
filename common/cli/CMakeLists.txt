
add_library(cli STATIC
    cli.c
)

target_include_directories(cli PUBLIC .)

add_subdirectory(usart)