set(EXECUTABLE serial_term)

add_executable_for(ARM_LINUX ${EXECUTABLE} ""
    main.c
)

target_link_options_for(ARM_LINUX ${EXECUTABLE} PRIVATE
    -pthread
)

target_link_libraries_for(ARM_LINUX ${EXECUTABLE}
    PRIVATE
        core
        driver
)
