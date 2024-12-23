#include <stdint.h>

extern uint8_t buffer[153600];

void draw_line(int x0, int y0, int x1, int y1, uint16_t color);
void draw_pixel(int x, int y, uint16_t color);
void clear_buffer();
void draw_buffer();