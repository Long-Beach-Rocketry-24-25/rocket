add_library(canbus STATIC
    st_bxcan.c
)

target_include_directories(canbus PUBLIC .)
target_link_libraries(canbus PUBLIC
    gpio
    timeout
)
