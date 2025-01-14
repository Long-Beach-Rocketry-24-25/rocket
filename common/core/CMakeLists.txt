
file(GLOB_RECURSE CORE_SRCS "*.c")

set(CORE_DIRS "")
get_directories(CORE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

add_library(core STATIC
    ${CORE_SRCS}
)

target_include_directories(core PUBLIC
    ${CORE_DIRS}
)

target_link_libraries(core PUBLIC
    driver
)