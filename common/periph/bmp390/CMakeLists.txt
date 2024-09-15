
add_library(bmp390 STATIC
    bmp390_i2c.c
)

target_include_directories(bmp390 PUBLIC .)
target_link_libraries(bmp390 PUBLIC
    i2c
)