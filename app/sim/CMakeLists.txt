set(EXECUTABLE sim)

add_executable_for(NATIVE ${EXECUTABLE} ""
    main.c
)

target_link_libraries_for(NATIVE ${EXECUTABLE}
    PRIVATE
        core
        driver
)

add_executable_for(ARM32 ${EXECUTABLE} ""
    main.c
)

target_link_libraries_for(ARM32 ${EXECUTABLE}
    PRIVATE
        core
        driver
)
