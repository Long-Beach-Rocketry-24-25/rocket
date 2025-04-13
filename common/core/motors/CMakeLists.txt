
add_library(motors STATIC 
    #encoder/drv8242_enc.c 
    #motor_control/dcm_control.c
    DC/fake_dcm.c
)

target_include_directories(motors PUBLIC 
    encoder
    DC
    motor_control
)

target_link_libraries(motors PUBLIC driver) 

add_tests(motors
    drv8242_dcm_test
)
