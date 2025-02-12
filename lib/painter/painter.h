#include <stdint.h>

extern uint8_t buffer[153600];

void draw_line(int x0, int y0, int x1, int y1, uint16_t color);
void draw_pixel(int x, int y, uint16_t color);
void draw_pixel_8bit(int x, int y, uint8_t color_h, uint8_t color_l);
void draw_image(uint8_t* image, uint16_t x, uint16_t y, uint16_t pos_x, uint16_t pos_y);
void clear_buffer();
// void clear_interlaced(uint8_t startLine);
void draw_buffer();