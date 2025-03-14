
add_library(comms STATIC
    uart_pipe.c
)

target_include_directories(comms PUBLIC .)
target_link_libraries(comms PUBLIC driver)