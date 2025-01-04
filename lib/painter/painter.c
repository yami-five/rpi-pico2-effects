#include "painter.h"
#include "pico/stdlib.h"
#include "DEV_Config.h"
#include "gfx.h"

#define WIDTH_DISPLAY 320
#define HEIGHT_DISPLAY 240
#define WIDTH_DOUBLED 640
#define HEIGHT_DOUBLED 480
#define ARRAY_SIZE 153600
#define WIDTH_HALF 160
#define HEIGHT_HALF 120

void draw_line(int x0, int y0, int x1, int y1, uint16_t color)
{
    
}

void draw_pixel(int x, int y, uint16_t color)
{
    int line_adr=x*HEIGHT_DOUBLED;
    int j2=y*2;
    buffer[line_adr+j2]=(color>>8)&0xff;
    buffer[line_adr+j2+1]=color&0xff;
}

void draw_pixel_8bit(int x, int y, uint8_t color_h, uint8_t color_l)
{
    int line_adr=x*HEIGHT_DOUBLED;
    int j2=y*2;
    buffer[line_adr+j2]=color_l;
    buffer[line_adr+j2+1]=color_h;
}

void draw_image(uint8_t* image, uint16_t image_x, uint16_t image_y, uint16_t pos_x, uint16_t pos_y)
{
    for (uint16_t y=0;y<image_y;y++)
    {
        if (y+pos_y>=240 || y+pos_y<0) continue;
        for (uint16_t x=0;x<image_x;x++)
        {
            if (x+pos_x>=320 || x+pos_x<0) continue;
            int addr=(y*image_x+x)*2;
            draw_pixel_8bit(x+pos_x,y+pos_y,image[addr],image[addr+1]);
        }
    }
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