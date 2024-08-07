cmake_minimum_required(VERSION 3.27)

project(myapp
    LANGUAGES CXX C ASM
)

if (BUILD_TARGET STREQUAL "Demo")
    add_compile_definitions(
        STM32H503xx=TRUE
        USE_HAL_DRIVER=TRUE
    )
    add_subdirectory(app/Demo)
elseif (BUILD_TARGET STREQUAL "can")
    add_compile_definitions(
        STM32H503xx=TRUE
        USE_HAL_DRIVER=TRUE
    )
    add_subdirectory(app/can)
endif()

add_subdirectory(mcu_support)