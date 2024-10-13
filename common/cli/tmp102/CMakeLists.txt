
add_library(read_tmp102 STATIC
    read_tmp102.c
)

target_include_directories(read_tmp102 PUBLIC .)
target_link_libraries(read_tmp102 PUBLIC
    cli
    tmp102
)