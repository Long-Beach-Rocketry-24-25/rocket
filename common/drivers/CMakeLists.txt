
add_library(drivers STATIC
    st_usart.c
)

target_include_directories(drivers PUBLIC .)
target_link_libraries(drivers PUBLIC
    timeout
)