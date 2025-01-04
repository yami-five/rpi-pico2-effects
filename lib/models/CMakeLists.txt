aux_source_directory(. DIR_MODELS_SRCS)

add_library(models ${DIR_MODELS_SRCS})

target_link_libraries(models PUBLIC Config)