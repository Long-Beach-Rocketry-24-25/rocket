
set(EXECUTABLE unit_tests)

add_executable_for(NATIVE ${EXECUTABLE} ""
    main.c
)

find_matching_targets("*_test")
