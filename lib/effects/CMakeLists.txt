aux_source_directory(. DIR_EFFECTS_SRCS)

add_library(effects ${DIR_EFFECTS_SRCS})

include_directories(../Config)
include_directories(../arithmetics)
include_directories(../painter)

target_link_libraries(effects PUBLIC
    Config
    arithmetics
    painter
    pico_stdlib
    pico_rand
    pico_multicore
)