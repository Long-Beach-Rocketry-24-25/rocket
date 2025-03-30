
add_library(motors STATIC
    DC/drv8242_dcm.c
)

target_include_directories(motors PUBLIC
    DC
)

target_link_libraries(motors PUBLIC driver)