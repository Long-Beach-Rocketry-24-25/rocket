
add_library(core INTERFACE)

target_link_libraries(core INTERFACE
    cli
    comms
    container
    gnc
    lbr_net
    math
    memory
    mission
    periph
    string
    timeout

)

target_link_libraries(core INTERFACE driver)

add_subdirectory(cli)
add_subdirectory(comms)
add_subdirectory(container)
add_subdirectory(gnc)
add_subdirectory(lbr_net)
add_subdirectory(math)
<<<<<<< HEAD
=======
add_subdirectory(memory)
add_subdirectory(mission)
>>>>>>> 6268c78 (experiment w factory funcs)
add_subdirectory(periph)
add_subdirectory(string)
add_subdirectory(timeout)

