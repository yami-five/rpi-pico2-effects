#include <stdint.h>
#include "pico/rand.h"
    
#define LUT_SIZE 360

void init_fire();
void draw_fire();
void plasma(uint32_t t);
float fast_sqrt(float x);
float fast_sin(float x);
void init_sin_lut();
static int16_t sin_lut[LUT_SIZE];
extern const unsigned char painting[];
extern unsigned char fire_arr[];
extern const uint16_t palette[16];
static uint16_t fire_lut1[240];
static uint16_t fire_lut2[240];
static uint16_t fire_lut3[240];
static uint16_t fire_lut4[320];
static uint16_t fire_lut5[320];