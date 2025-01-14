file(GLOB_RECURSE FREERTOS_APP_SRCS "*.c")

set(FREERTOS_APP_DIRS "")
get_directories(FREERTOS_APP_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

add_library(freertos_app STATIC
    ${FREERTOS_APP_SRCS}
)

target_include_directories(freertos_app PUBLIC
    ${FREERTOS_APP_DIRS}
)

target_link_libraries(freertos_app PUBLIC
    core
    driver
    freertos_kernel
)