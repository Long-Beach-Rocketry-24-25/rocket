if (TARGET_DEVICE MATCHES "NATIVE")
    add_compile_definitions(
        NATIVE=TRUE
    )
    add_subdirectory(platform/native)
elseif (MCU_NAME MATCHES "^(STM32F7)[0-9]+xx")
    add_subdirectory(platform/stm32f7)
elseif (MCU_NAME MATCHES "^(STM32H5)[0-9]+xx")
    add_subdirectory(platform/stm32h5)
elseif (MCU_NAME MATCHES "^(STM32L4)[0-9]+xx")
    add_subdirectory(platform/stm32l4)
endif()

add_library(driver INTERFACE)

target_include_directories(driver INTERFACE
    bus
    ota
    memory
)

target_link_libraries(driver INTERFACE
    hal
)