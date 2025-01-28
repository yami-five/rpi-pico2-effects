aux_source_directory(. DIR_MODELS_SRCS)

add_library(models ${DIR_MODELS_SRCS})

include_directories(../Config)
include_directories(../fpa)

target_link_libraries(models PUBLIC 
    Config 
    fpa
)