aux_source_directory(. DIR_effects_SRCS)

add_library(rasterizer ${DIR_effects_SRCS})

include_directories(../Config)
include_directories(../effects)
include_directories(../painter)

target_link_libraries(rasterizer PUBLIC 
    effects
    painter
    pico_stdlib
    pico_rand
    pico_multicore
    Config
)