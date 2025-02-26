#include "DEV_Config.h"
#include "effects.h"
#include "fpa.h"
#include "hardware/divider.h"
#include "hardware/sync.h"
#include "painter.h"
#include "pico/float.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include <math.h>
#include <stdint.h>

#define WIDTH_DISPLAY 320
#define HEIGHT_DISPLAY 240
#define WIDTH_DOUBLED 640
#define HEIGHT_DOUBLED 480
#define ARRAY_SIZE 153600
#define WIDTH_HALF 160
#define HEIGHT_HALF 120
#define FIXED_128 floatToFixed(128.0f)
#define FIXED_16 floatToFixed(16.0f)
#define FIXED_0128 floatToFixed(0.128f)
#define FIXED_0008 floatToFixed(0.008f)
#define FIXED_0004 floatToFixed(0.004f)
#define FIXED_0001 floatToFixed(0.001f)

void plasma(uint32_t t)
{
    int32_t fst = t * fixedMul(PI2_FIXED, FIXED_0001);
    uint8_t line[ARRAY_SIZE];

    for (uint16_t i = 0; i < WIDTH_DISPLAY; i += 2)
    {
        uint32_t i2 = i * i * SCALE_FACTOR;
        uint32_t imw = i - WIDTH_HALF;
        imw = imw * imw;
        float line1 = fixedMul((i * FIXED_0128 + fst), PI2_FIXED);
        line1 = fixedMul(FIXED_16, line1) + FIXED_128;
        uint32_t line_adr = i * HEIGHT_DOUBLED;
        for (uint16_t j = 0; j < HEIGHT_DISPLAY; j += 2)
        {
            uint32_t jmh = j - HEIGHT_HALF;
            jmh = jmh * jmh;
            uint32_t color_index1 = line1 + FIXED_128;
            
            color_index1 += fixedMul(
                FIXED_16, fastSin(fixedMul(j * FIXED_0008 + fst, PI2_FIXED)));

            color_index1 += FIXED_128 + fixedMul(
                FIXED_16, fastSin(
                    fixedMul((
                        fixedMul(
                            fastSqrt(
                                imw * SCALE_FACTOR + jmh * SCALE_FACTOR
                            ), FIXED_0128
                        ) + fst
                    ), PI2_FIXED)
                )
            );

            color_index1 += FIXED_128 + fixedMul(
                FIXED_16, fastSin(
                    fixedMul((
                        fastSqrt(i2 + j * j * SCALE_FACTOR) * FIXED_0004 + fst
                    ), PI2_FIXED)
                )
            );

            color_index1 /= SCALE_FACTOR;

            uint16_t color = palette[(color_index1 / 4) % 16];
            draw_pixel(i, j, color);
            draw_pixel(i + 1, j, color);
            draw_pixel(i, j + 1, color);
            draw_pixel(i + 1, j + 1, color);
        }
    }
}

void init_fire()
{
    for (uint16_t i = 0; i < WIDTH_DISPLAY; i++)
    {
        fire_lut1[i] = (i + WIDTH_DISPLAY) % WIDTH_DISPLAY;
        fire_lut2[i] = (i + 1) % WIDTH_DISPLAY;
        fire_lut3[i] = i % WIDTH_DISPLAY;
    }
    uint8_t line[ARRAY_SIZE];
    for (uint16_t i = 0; i < HEIGHT_DISPLAY; i++)
    {
        for (uint16_t j = 0; j < WIDTH_DISPLAY; j++)
        {
            uint32_t line_adr = j * HEIGHT_DOUBLED;
            uint32_t addr = (i * WIDTH_DISPLAY + j);
            uint8_t color_index1;
            if (i < HEIGHT_DISPLAY - 1)
                color_index1 = 0;
            else
                color_index1 = get_rand_32() % 32;
            fire_arr[addr] = color_index1;
            uint16_t color = palette[color_index1];
            draw_pixel(i, j, color);
        }
    }
}

void draw_fire()
{
    uint8_t line[ARRAY_SIZE];
    uint16_t fire_floor = HEIGHT_DISPLAY - 1;
    for (uint16_t j = 0; j < WIDTH_DISPLAY; j++)
    {
        uint8_t color_index1 = get_rand_32() % 32;
        uint32_t addr = (fire_floor * WIDTH_DISPLAY + j);
        fire_arr[addr] = color_index1;
        uint16_t color = palette[color_index1];
        line[j * HEIGHT_DOUBLED + HEIGHT_DOUBLED + 1] = color & 0xff;
        line[j * HEIGHT_DOUBLED + HEIGHT_DOUBLED] = (color >> 8) & 0xff;
    }
    for (uint16_t y = 0; y < HEIGHT_DISPLAY - 1; y++)
    {
        uint32_t yp2modhmw = ((y + 2) % HEIGHT_DISPLAY) * WIDTH_DISPLAY;
        uint32_t yp4modhmw = ((y + 4) % HEIGHT_DISPLAY) * WIDTH_DISPLAY;
        uint32_t yp6modhmw = ((y + 6) % HEIGHT_DISPLAY) * WIDTH_DISPLAY;
        for (uint16_t x = 0; x < WIDTH_DISPLAY; x++)
        {
            uint32_t line_adr = x * HEIGHT_DOUBLED;
            uint16_t xmodw = fire_lut3[x];
            uint8_t color_index1 = ((
                                        fire_arr[yp2modhmw + fire_lut1[x]] + fire_arr[yp4modhmw + xmodw] + fire_arr[yp2modhmw + fire_lut2[x]] + fire_arr[yp6modhmw + xmodw]) /
                                    4) %
                                   32;
            fire_arr[y * WIDTH_DISPLAY + x] = color_index1;
            uint16_t color = palette[color_index1];
            draw_pixel(x, HEIGHT_DISPLAY - y, color);
        }
    }
}
static int16_t sin_lut[LUT_SIZE];
uint8_t fire_arr[76800];
static uint16_t fire_lut1[320];
static uint16_t fire_lut2[320];
static uint16_t fire_lut3[320];
const uint16_t palette[16] =
    {
        0x0000,
        0x2005,
        0x4847,
        0x7088,
        0x8867,
        0xA845,
        0xB905,
        0xCA45,
        0xE326,
        0xE449,
        0xEDAB,
        0xEEAF,
        0xF775,
        0xFFFB,
        0xFFDE,
        0xFFFF};
