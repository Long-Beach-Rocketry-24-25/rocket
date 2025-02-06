
add_library(timeout STATIC
    forever_timer.c
    retry_timer.c
)

target_include_directories(timeout PUBLIC .)
