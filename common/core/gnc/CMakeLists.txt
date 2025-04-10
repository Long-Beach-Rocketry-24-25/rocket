
add_library(gnc STATIC
    acceleration.c
    altitude.c
<<<<<<< HEAD
)

target_include_directories(gnc PUBLIC .)
target_link_libraries(gnc PUBLIC math)
=======
    simple_kalman.c
)

target_include_directories(gnc PUBLIC .)
target_link_libraries(gnc PUBLIC math)
>>>>>>> 651364b (Nf flight algo)
