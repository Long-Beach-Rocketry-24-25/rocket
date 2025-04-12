
add_library(motors STATIC 
    encoder/drv8242_enc.c 
    DC/fake_dcm.c 
    motor_control/dcm_control.c
)

target_include_directories(motors PUBLIC.)

target_link_libraries(motors PUBLIC driver)