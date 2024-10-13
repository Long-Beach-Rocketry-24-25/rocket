
add_library(read_bno055 STATIC
    read_bno055.c
)

target_include_directories(read_bno055 PUBLIC .)
target_link_libraries(read_bno055 PUBLIC
    cli
    bno055
)