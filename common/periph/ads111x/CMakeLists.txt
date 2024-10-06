add_library(ads111x STATIC
    ads111x.c
)

target_include_directories(ads111x PUBLIC .)
target_link_libraries(ads111x PUBLIC
    i2c
)