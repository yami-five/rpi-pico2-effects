aux_source_directory(. DIR_RASTERIZER_SRCS)

add_library(rasterizer ${DIR_RASTERIZER_SRCS})

include_directories(../Config)
include_directories(../effects)
include_directories(../models)
include_directories(../painter)
include_directories(../fpa)

target_link_libraries(rasterizer PUBLIC 
    effects
    models
    painter
    pico_stdlib
    pico_rand
    pico_multicore
    Config
    fpa
)