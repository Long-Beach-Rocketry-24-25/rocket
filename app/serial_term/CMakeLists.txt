set(EXECUTABLE serial_term)

add_executable_for(ARM32 ${EXECUTABLE} ""
    main.c
)

target_link_options_for(ARM32 ${EXECUTABLE} PRIVATE
    -pthread
)

target_link_libraries_for(ARM32 ${EXECUTABLE}
    PRIVATE
        core
        driver
)
