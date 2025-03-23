
add_library(math STATIC
    altitude.c
    matrix.c
    simple_kalman.c
)

target_include_directories(math PUBLIC .)
