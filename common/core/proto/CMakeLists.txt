
add_library(proto STATIC
    example.pb.c
)

target_include_directories(proto PUBLIC .)

target_link_libraries(proto
    PUBLIC
    protobuf-nanopb-static
)