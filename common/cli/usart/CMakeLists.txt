
add_library(usart_cli STATIC
    usart_cli.c
)

target_include_directories(usart_cli PUBLIC .)
target_link_libraries(usart_cli
PUBLIC
    cli
    drivers
    timeout
    container
PRIVATE
    freertos_kernel
)