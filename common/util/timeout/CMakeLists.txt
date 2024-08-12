
add_library(timeout STATIC
    retry_timer.c
)
target_include_directories(timeout PUBLIC .)