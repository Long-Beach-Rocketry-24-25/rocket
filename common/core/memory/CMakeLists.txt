add_library(memory STATIC
    dalloc.c
    stalloc.c
)

target_include_directories(memory PUBLIC .)

add_tests(memory
    alloc_test
)