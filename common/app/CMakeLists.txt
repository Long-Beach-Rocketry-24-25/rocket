if (NOT "${FREERTOS_PORT}" STREQUAL "")
    add_subdirectory(freertos)
endif()

add_subdirectory(generic)