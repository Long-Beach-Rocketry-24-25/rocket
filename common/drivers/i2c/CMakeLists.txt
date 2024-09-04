add_library(i2c STATIC
    st_i2c.c
)

target_include_directories(i2c PUBLIC .)
target_link_libraries(i2c PUBLIC
    timeout
)