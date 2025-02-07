add_library(hal INTERFACE)

target_include_directories(hal INTERFACE
    bus
    memory
    .
)
target_link_libraries(hal INTERFACE
    core
)
