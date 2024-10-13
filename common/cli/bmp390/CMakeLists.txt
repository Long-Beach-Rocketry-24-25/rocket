
add_library(read_bmp390 STATIC
    read_bmp390.c
)

target_include_directories(read_bmp390 PUBLIC .)
target_link_libraries(read_bmp390 PUBLIC
    cli
    bmp390
    gnc
)