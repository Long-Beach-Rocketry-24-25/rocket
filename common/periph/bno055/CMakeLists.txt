
add_library(bno055 STATIC
    bno055.c
)

target_include_directories(bno055 PUBLIC .)
target_link_libraries(bno055 PUBLIC
    i2c
)