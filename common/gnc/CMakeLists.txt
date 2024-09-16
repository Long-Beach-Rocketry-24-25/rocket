# WARNING: THIS IS A HARDWARE AGNOSTIC FOLDER - DO NOT ADD MCU CODE.

add_library(gnc STATIC
    gnc.c
)

target_include_directories(gnc PUBLIC .)