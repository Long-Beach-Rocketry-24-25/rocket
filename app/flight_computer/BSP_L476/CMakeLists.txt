add_library(flight_computer_bsp STATIC
    flight_computer_bsp.c
    hal_timebase.c
)

target_include_directories(flight_computer_bsp PUBLIC .)
target_link_libraries(flight_computer_bsp
    PUBLIC
        app
        core
        driver
    INTERFACE
        freertos_kernel
)
