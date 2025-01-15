cmake_minimum_required(VERSION 3.27)

project(myapp
    LANGUAGES CXX C ASM
)

include(cmake/build-util.cmake)

if(MCU_NAME MATCHES "^(STM32).*")
    message("Found STM32 Device!")
    add_compile_definitions(
        ${MCU_NAME}=TRUE
        USE_HAL_DRIVER=TRUE
    )
endif()

if ("${TARGET_APP}" STREQUAL "")
    add_subdirectory(app)
else()
    add_subdirectory(app/${TARGET_APP})
endif()

add_subdirectory(common)
add_subdirectory(mcu_support)