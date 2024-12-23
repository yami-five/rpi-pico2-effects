#include <stdint.h>

extern uint8_t buffer[153600];

void draw_pixel(int x, int y, uint16_t color);
void clear_buffer();
void draw_buffer();