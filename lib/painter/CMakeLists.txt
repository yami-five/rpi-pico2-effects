aux_source_directory(. DIR_effects_SRCS)

add_library(painter ${DIR_effects_SRCS})

include_directories(../Config)
include_directories(../effects)

target_link_libraries(painter PUBLIC 
    effects
    pico_stdlib
    pico_rand
    pico_multicore
    Config
)