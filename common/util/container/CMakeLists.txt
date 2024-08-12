
add_library(container STATIC
    ring_buffer.c
)
target_include_directories(container PUBLIC .)