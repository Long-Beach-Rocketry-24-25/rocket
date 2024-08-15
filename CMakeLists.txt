cmake_minimum_required(VERSION 3.27)

project(myapp
    LANGUAGES CXX C ASM
)

add_compile_definitions(
    ${MCU_NAME}=TRUE
    USE_HAL_DRIVER=TRUE
)

if (MCU_NAME MATCHES "^(STM32H5)[0-9]+xx")
    add_subdirectory(mcu_support/stm32/h5xx)
endif()

if (MCU_NAME MATCHES "^(STM32F7)[0-9]+xx")
    add_subdirectory(mcu_support/stm32/f7xx)
endif()

if (BUILD_TARGET STREQUAL "Demo")
    add_subdirectory(app/Demo)
elseif (BUILD_TARGET STREQUAL "echo")
    add_subdirectory(app/echo)
elseif (BUILD_TARGET STREQUAL "cli_f7")
    add_subdirectory(app/cli_f7)
endif()

add_subdirectory(mcu_support/cmsis)
add_subdirectory(common)