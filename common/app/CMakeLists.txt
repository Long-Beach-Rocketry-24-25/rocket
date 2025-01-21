add_library(app INTERFACE)

if (NOT "${FREERTOS_PORT}" STREQUAL "")
    add_subdirectory(freertos)
    target_link_libraries(app INTERFACE freertos_app)
endif()

add_subdirectory(generic)
target_link_libraries(app INTERFACE generic_app)