aux_source_directory(. DIR_sd_driver_SRCS)

include_directories(../Config)

add_library(sd_driver ${DIR_sd_driver_SRCS})

target_link_libraries(sd_driver PUBLIC pico_stdlib Config)