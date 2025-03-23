
add_library(math STATIC
    altitude.c
    kalman.c
    matrix.c
)

target_include_directories(math PUBLIC .)
