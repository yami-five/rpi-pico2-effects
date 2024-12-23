#include "painter.h"
#include "pico/stdlib.h"
#include "DEV_Config.h"

#define WIDTH_DISPLAY 320
#define HEIGHT_DISPLAY 240
#define WIDTH_DOUBLED 640
#define HEIGHT_DOUBLED 480
#define ARRAY_SIZE 153600
#define WIDTH_HALF 160
#define HEIGHT_HALF 120

void draw_pixel(int y, int x, uint16_t color)
{
    uint32_t line_adr=x*HEIGHT_DOUBLED;
    uint16_t j2=y*2;
    buffer[line_adr+j2]=(color>>8)&0xff;
    buffer[line_adr+j2+1]=color&0xff;
}

void clear_buffer()
{
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        buffer[i] = 0x00;
    }
}

void draw_buffer()
{
    DEV_Digital_Write(LCD_DC_PIN, 1);
    DEV_Digital_Write(LCD_CS_PIN, 0);
    DEV_SPI_Write_nByte(buffer, ARRAY_SIZE);
    DEV_Digital_Write(LCD_CS_PIN, 1);
}

uint8_t buffer[153600];