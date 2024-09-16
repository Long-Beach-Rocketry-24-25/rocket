add_subdirectory(app)
add_subdirectory(gnc)

if (NOT ${SIM_PLATFORM})
    add_subdirectory(cli)
    add_subdirectory(drivers)
    add_subdirectory(periph)
    add_subdirectory(util)
endif()