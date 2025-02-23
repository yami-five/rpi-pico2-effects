aux_source_directory(. DIR_CAMERA_SRCS)

include_directories(../arithmetics)

add_library(camera ${DIR_CAMERA_SRCS})

target_link_libraries(camera PUBLIC pico_stdlib arithmetics)