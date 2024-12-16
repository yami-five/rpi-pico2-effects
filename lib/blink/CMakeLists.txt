aux_source_directory(. DIR_LCD_SRCS)
set(PICO_DEOPTIMIZED_DEBUG=1)
add_library(blink blink.c blink.h)
target_link_libraries(blink pico_stdlib)