file(GLOB_RECURSE NATIVE_APP_SRCS "*.c")

add_library(native_app STATIC
    ${NATIVE_APP_SRCS}
)

target_include_directories(native_app PUBLIC
    navigation
)

target_link_libraries(native_app PUBLIC
    core
    driver
    generic_app
)