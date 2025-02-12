aux_source_directory(. DIR_PAINTER_SRCS)

add_library(painter ${DIR_PAINTER_SRCS})

include_directories(../Config)
include_directories(../effects)
include_directories(../gfx)

target_link_libraries(painter PUBLIC 
    effects
    gfx
    pico_stdlib
    pico_rand
    pico_multicore
    Config
    hardware_dma
)