set(EXECUTABLE logging)

add_executable_for(NATIVE ${EXECUTABLE} ""
    main.c
)

target_link_libraries_for(NATIVE ${EXECUTABLE}
    PRIVATE
        app
        core
        driver
)

configure_file(navdata.log navdata.log COPYONLY)