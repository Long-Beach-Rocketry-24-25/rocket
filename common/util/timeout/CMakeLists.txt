
add_library(timeout STATIC
    retry_timer.c
    forever_timer.c
    freertos_timer.c
)
target_include_directories(timeout PUBLIC .)

target_link_libraries(timeout PUBLIC
    freertos_kernel
)