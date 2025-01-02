aux_source_directory(. DIR_effects_SRCS)

add_library(effects ${DIR_effects_SRCS})

include_directories(../Config)
include_directories(../painter)

target_link_libraries(effects PUBLIC 
    painter
    pico_stdlib
    pico_rand
    pico_multicore
    Config
)