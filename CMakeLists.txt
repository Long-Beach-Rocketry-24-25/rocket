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
elseif (BUILD_TARGET STREQUAL "usart")
    add_subdirectory(app/usart)
endif()

add_subdirectory(mcu_support/stm32/h5xx)
add_subdirectory(mcu_support/cmsis)
add_subdirectory(common)