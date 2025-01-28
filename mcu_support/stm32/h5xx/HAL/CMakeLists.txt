FILE(GLOB SRC_FILES *.c)

add_library(st_h5_hal STATIC
    ${SRC_FILES}
)

target_link_libraries(st_h5_hal PUBLIC
    st_h5_support
)
target_include_directories(st_h5_hal PUBLIC .)