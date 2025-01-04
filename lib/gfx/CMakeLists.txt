aux_source_directory(. DIR_GFX_SRCS)

add_library(gfx ${DIR_GFX_SRCS})

target_link_libraries(gfx PUBLIC Config)