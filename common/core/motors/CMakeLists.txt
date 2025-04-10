
# add_library(motor STATIC
#     encoder/drv8242_enc.c
#     DC/drv8242_dcm.c
# )

# target_include_directories(motor PUBLIC .)

add_tests(motor
    dc_control_test
    drv8242_enc_test
)