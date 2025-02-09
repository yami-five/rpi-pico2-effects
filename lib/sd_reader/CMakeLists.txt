aux_source_directory(. DIR_SD_READER_SRCS)

include_directories(../Config)
include_directories(../fatfs)

add_library(sd_reader ${DIR_SD_READER_SRCS})

target_link_libraries(sd_reader PUBLIC pico_stdlib Config fatfs)