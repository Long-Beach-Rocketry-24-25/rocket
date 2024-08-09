cmake_minimum_required(VERSION 3.27)

project(myapp
    LANGUAGES CXX C ASM
)

add_compile_definitions(
    ${MCU_NAME}=TRUE
    USE_HAL_DRIVER=TRUE
)

if (BUILD_TARGET STREQUAL "Demo")
    add_subdirectory(app/Demo)
elseif (BUILD_TARGET STREQUAL "can")
    add_subdirectory(app/can)
endif()

add_subdirectory(mcu_support)