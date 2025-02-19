aux_source_directory(. DIR_BLINK_SRCS)

add_library(blink ${DIR_BLINK_SRCS})
target_link_libraries(blink PUBLIC pico_stdlib)