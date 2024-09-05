
add_library(tmp102 STATIC
    tmp102.c
)

target_include_directories(tmp102 PUBLIC .)
target_link_libraries(tmp102 PUBLIC
    i2c
)