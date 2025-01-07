aux_source_directory(. DIR_FPA_SRCS)

add_library(fpa ${DIR_FPA_SRCS})

target_link_libraries(fpa PUBLIC 
)