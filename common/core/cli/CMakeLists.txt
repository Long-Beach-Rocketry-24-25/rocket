
add_library(cli STATIC
    cli.c
    send.c
)

target_include_directories(cli PUBLIC .)
