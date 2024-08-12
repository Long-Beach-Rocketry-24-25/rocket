
add_library(cli STATIC
    cli.c
)

target_include_directories(cli PUBLIC .)
target_link_libraries(cli 
    PRIVATE 
        freertos_kernel
)