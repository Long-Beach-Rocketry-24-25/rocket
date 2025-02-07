
add_library(timeout STATIC
    forever_timer.c
    retry_timer.c
)

target_include_directories(timeout PUBLIC .)

add_tests(timeout
    retry_timer_test
)
