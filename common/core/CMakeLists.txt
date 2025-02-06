
add_library(core INTERFACE)

target_link_libraries(core INTERFACE
    cli
    container
    math
    mission
    periph
    string
    timeout
)

target_link_libraries(core INTERFACE driver)

add_subdirectory(cli)
add_subdirectory(container)
add_subdirectory(math)
add_subdirectory(mission)
add_subdirectory(periph)
add_subdirectory(string)
add_subdirectory(timeout)
