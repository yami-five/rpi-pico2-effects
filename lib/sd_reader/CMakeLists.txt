aux_source_directory(. DIR_SD_READER_SRCS)

add_library(sd_reader ${DIR_SD_READER_SRCS})

include_directories(../Config)
include_directories(../fpa)
include_directories(../fatfs)

target_link_libraries(sd_reader PUBLIC pico_stdlib Config fatfs fpa)