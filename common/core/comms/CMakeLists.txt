
add_library(comms STATIC
    lbr_net.c
    uart_pipe.c
)


target_include_directories(comms 
    PUBLIC
    .
    ${CMAKE_SOURCE_DIR}/external/nanopb
)

target_link_libraries(comms
    PUBLIC
    driver
    protobuf-nanopb-static
)

add_tests(comms
    lbr_net_test
)
