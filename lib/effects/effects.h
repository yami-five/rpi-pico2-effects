#include <stdint.h>
#include "pico/rand.h"
    
#define LUT_SIZE 360

void init_fire();
void draw_fire();
void plasma(uint32_t t);
static int16_t sin_lut[LUT_SIZE];
extern const unsigned char painting[];
extern uint8_t fire_arr[76800];
extern const uint16_t palette[16];
static uint16_t fire_lut1[320];
static uint16_t fire_lut2[320];
static uint16_t fire_lut3[320];