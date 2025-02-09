aux_source_directory(. DIR_FATFS_SRCS)

add_library(fatfs ${DIR_FATFS_SRCS})

include_directories(../Config)
include_directories(../sd_driver)


target_link_libraries(fatfs PUBLIC Config sd_driver)