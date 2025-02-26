aux_source_directory(. DIR_SD_READER_SRCS)

add_library(sd_reader ${DIR_SD_READER_SRCS})

include_directories(../blink)
include_directories(../Config)
include_directories(../arithmetics)
include_directories(../fatfs)
include_directories(../painter)

target_link_libraries(sd_reader PUBLIC pico_stdlib blink Config fatfs arithmetics painter)