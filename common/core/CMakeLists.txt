
add_library(core INTERFACE)

target_link_libraries(core INTERFACE
    cli
    comms
    container
    math
    mission
    periph
    proto
    string
    timeout

)

target_link_libraries(core INTERFACE driver)

add_subdirectory(cli)
add_subdirectory(comms)
add_subdirectory(container)
add_subdirectory(math)
add_subdirectory(mission)
add_subdirectory(periph)
add_subdirectory(proto)
add_subdirectory(string)
add_subdirectory(timeout)

