
add_library(math STATIC
    matrix.c
    quaternion.c
)

target_include_directories(math PUBLIC .)

add_tests(math
    quaternion_test
)
