aux_source_directory(. DIR_LCD_SRCS)

include_directories(../Config)
include_directories(../GUI)
include_directories(../Fonts)

add_library(LCD ${DIR_LCD_SRCS})
target_link_libraries(LCD PUBLIC GUI Fonts Config)