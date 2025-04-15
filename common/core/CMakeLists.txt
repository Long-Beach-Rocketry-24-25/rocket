
add_library(core INTERFACE)

target_link_libraries(core INTERFACE
    cli
    comms
    container
    lbr_net
    math
    mission
    periph
    string
    switches
    timeout

)

target_link_libraries(core INTERFACE driver)

add_subdirectory(cli)
add_subdirectory(comms)
add_subdirectory(container)
add_subdirectory(lbr_net)
add_subdirectory(math)
add_subdirectory(mission)
add_subdirectory(periph)
add_subdirectory(string)
add_subdirectory(switches)
add_subdirectory(timeout)

