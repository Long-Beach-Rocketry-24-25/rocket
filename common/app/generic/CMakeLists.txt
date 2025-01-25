file(GLOB_RECURSE GENERIC_APP_SRCS "*.c")

add_library(generic_app STATIC
    ${GENERIC_APP_SRCS}
)

target_include_directories(generic_app PUBLIC
    commands
    logging
    logging/builders
    logging/subscribers
    navigation
)

target_link_libraries(generic_app PUBLIC
    core
    driver
)