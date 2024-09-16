set(EXECUTABLE sim)

add_executable(${EXECUTABLE}
    main.c
)

target_link_libraries(${EXECUTABLE}
    PRIVATE
        gnc
)