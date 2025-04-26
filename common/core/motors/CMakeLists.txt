
add_library(motors
    nidec_24h.c
)

target_include_directories(motors PUBLIC .)
target_link_libraries(motors PUBLIC
    driver
    memory
)