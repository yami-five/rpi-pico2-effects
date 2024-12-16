aux_source_directory(. DIR_effects_SRCS)

add_library(effects ${DIR_effects_SRCS})

include_directories(../Config)

target_link_libraries(effects PUBLIC 
    pico_stdlib
    pico_rand
    pico_multicore
    Config
)