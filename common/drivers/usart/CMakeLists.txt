
add_library(usart STATIC
    st_usart.c
)

target_include_directories(usart PUBLIC .)
target_link_libraries(usart PUBLIC
    timeout
)