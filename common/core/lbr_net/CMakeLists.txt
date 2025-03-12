add_library(lbr_net STATIC
    lbr_net.c
)

target_include_directories(lbr_net PUBLIC .)

add_tests(lbr_net
    format_test
)