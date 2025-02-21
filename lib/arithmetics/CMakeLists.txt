aux_source_directory(. DIR_ARITHMETICS_SRCS)

add_library(arithmetics ${DIR_ARITHMETICS_SRCS})

target_link_libraries(arithmetics PUBLIC)