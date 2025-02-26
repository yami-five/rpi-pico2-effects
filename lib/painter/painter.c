#include "painter.h"
#include "pico/stdlib.h"
#include "DEV_Config.h"
#include "gfx.h"
#include <stdlib.h>
#include "hardware/dma.h"

#define WIDTH_DISPLAY 320
#define HEIGHT_DISPLAY 240
#define WIDTH_DOUBLED 640
#define HEIGHT_DOUBLED 480
#define ARRAY_SIZE 153600
#define WIDTH_HALF 160
#define HEIGHT_HALF 120
#define BUFFER_SIZE 153600

uint8_t buffer[BUFFER_SIZE];
int dma_channel;

void init_dma()
{
    dma_channel = dma_claim_unused_channel(true);
    dma_channel_config config = dma_channel_get_default_config(dma_channel);
    channel_config_set_transfer_data_size(&config, DMA_SIZE_8);
    channel_config_set_dreq(&config, spi_get_dreq(spi1, true));
    dma_channel_configure(
        dma_channel,
        &config,
        &spi_get_hw(spi1)->dr,
        buffer,
        BUFFER_SIZE,
        false);
}

void draw_line(int x0, int y0, int x1, int y1, uint16_t color)
{
}

void draw_pixel(int x, int y, uint16_t color)
{
    int line_adr = x * HEIGHT_DOUBLED;
    int j2 = y * 2;
    buffer[line_adr + j2] = (color >> 8) & 0xff;
    buffer[line_adr + j2 + 1] = color & 0xff;
}

void draw_pixel_8bit(int x, int y, uint8_t color_h, uint8_t color_l)
{
    int line_adr = x * HEIGHT_DOUBLED;
    int j2 = y * 2;
    buffer[line_adr + j2] = color_l;
    buffer[line_adr + j2 + 1] = color_h;
}

void draw_image(uint16_t *image, uint16_t image_x, uint16_t image_y, uint16_t pos_x, uint16_t pos_y)
{
    for (uint16_t y = 0; y < image_y; y++)
    {
        if (y + pos_y >= 240 || y + pos_y < 0)
            continue;
        for (uint16_t x = 0; x < image_x; x++)
        {
            if (x + pos_x >= 320 || x + pos_x < 0)
                continue;
            int addr = y * image_x + x;
            // draw_pixel_8bit(x + pos_x, y + pos_y, image[addr], image[addr + 1]);
            draw_pixel(x + pos_x, y + pos_y, image[addr]);
        }
    }
}

void clear_buffer()
{
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        buffer[i] = 0;
    }
}

void lcd_refresh()
{
    DEV_Digital_Write(LCD_CS_PIN, 0);
    DEV_Digital_Write(LCD_DC_PIN, 0);
    DEV_SPI_WriteByte(0x2C);
    DEV_Digital_Write(LCD_DC_PIN, 1);
    DEV_Digital_Write(LCD_CS_PIN, 1);
}

void draw_buffer()
{
    DEV_Digital_Write(LCD_DC_PIN, 1);
    DEV_Digital_Write(LCD_CS_PIN, 0);
    dma_channel_transfer_from_buffer_now(dma_channel, buffer, BUFFER_SIZE);
    dma_channel_wait_for_finish_blocking(dma_channel);
    lcd_refresh();
    DEV_Digital_Write(LCD_CS_PIN, 1);
}

void draw_buff_line(uint8_t *line)
{
    DEV_Digital_Write(LCD_DC_PIN, 1);
    DEV_Digital_Write(LCD_CS_PIN, 0);
    dma_channel_transfer_from_buffer_now(dma_channel, line, WIDTH_DISPLAY*2);
    dma_channel_wait_for_finish_blocking(dma_channel);
    DEV_Digital_Write(LCD_CS_PIN, 1);
}