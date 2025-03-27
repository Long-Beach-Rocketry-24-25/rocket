
add_library(math STATIC
    altitude.c
    matrix.c
    quaternion.c
    simple_kalman.c
)

target_include_directories(math PUBLIC .)

add_tests(math
    quaternion_test
)
