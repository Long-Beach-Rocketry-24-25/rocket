
add_library(i2c_access STATIC
    i2c_access.c
)

target_include_directories(i2c_access PUBLIC .)
target_link_libraries(i2c_access PUBLIC
    cli
    i2c
    str_util
)