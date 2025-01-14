set(EXECUTABLE sim)

add_executable_for(NATIVE ${EXECUTABLE} ""
    main.c
)

target_link_libraries_for(NATIVE ${EXECUTABLE}
    PRIVATE
        core
        driver
)
