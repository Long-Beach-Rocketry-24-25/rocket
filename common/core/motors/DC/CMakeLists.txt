add_library(DC STATIC
    fake_dcm.c
)

target_include_directories(DC PUBLIC .)

add_tests(DC
    drv8242_dcm_test
)