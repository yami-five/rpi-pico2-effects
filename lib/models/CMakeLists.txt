aux_source_directory(. DIR_MODELS_SRCS)

add_library(models ${DIR_MODELS_SRCS})

include_directories(../blink)
include_directories(../Config)
include_directories(../fpa)
include_directories(../fatfs)
include_directories(../sd_reader)

target_link_libraries(models PUBLIC
    blink
    Config 
    fpa
    fatfs
    sd_reader
)