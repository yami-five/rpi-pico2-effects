#include <stdint.h>
#include <DEV_Config.h>

#define LCD_RST_PIN  12
#define LCD_DC_PIN   8
#define LCD_BL_PIN   13
    
#define LCD_CS_PIN   9
#define LCD_CLK_PIN  10
#define LCD_MOSI_PIN 11
    
#define LCD_SCL_PIN  7
#define LCD_SDA_PIN  6

#define TE_PIN 14

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240

#define HORIZONTAL 0
#define VERTICAL 1

static void send_command(uint8_t Reg)
{
    DEV_Digital_Write(LCD_DC_PIN, 0);
    DEV_Digital_Write(LCD_CS_PIN, 0);
    DEV_SPI_WriteByte(Reg);
    DEV_Digital_Write(LCD_CS_PIN, 1);
}

static void send_data_8bit(uint8_t Data)
{
    DEV_Digital_Write(LCD_DC_PIN, 1);
    DEV_Digital_Write(LCD_CS_PIN, 0);
    DEV_SPI_WriteByte(Data);
    DEV_Digital_Write(LCD_CS_PIN, 1);
}

static void send_data_16bit(uint16_t Data)
{
    DEV_Digital_Write(LCD_DC_PIN, 1);
    DEV_Digital_Write(LCD_CS_PIN, 0);
    DEV_SPI_WriteByte((Data >> 8) & 0xFF);
    DEV_SPI_WriteByte(Data & 0xFF);
    DEV_Digital_Write(LCD_CS_PIN, 1);
}

static void set_attributes(uint8_t Scan_dir)
{
    uint8_t MemoryAccessReg = 0x00;
    if(Scan_dir == HORIZONTAL) {
        MemoryAccessReg = 0X70;
    } else {
        MemoryAccessReg = 0X00;
    }

    // Set the read / write scan direction of the frame memory
    send_command(0x36); //MX, MY, RGB mode
    send_data_8bit(MemoryAccessReg);	//0x08 set RGB
}

void init_lcd(uint8_t Scan_dir)
{
    //set pwm
    DEV_SET_PWM(100);
    //Hardware reset
    lcd_reset();

    //Set the resolution and scanning method of the screen
    set_attributes(Scan_dir);
    
    //Set the initialization register
    init_reg();
}

void lcd_reset(void)
{
    DEV_Digital_Write(LCD_RST_PIN, 1);
    DEV_Delay_ms(100);
    DEV_Digital_Write(LCD_RST_PIN, 0);
    DEV_Delay_ms(100);
    DEV_Digital_Write(LCD_RST_PIN, 1);
    DEV_Delay_ms(100);
}

void init_reg()
{
    send_command(0x36);
	send_data_8bit(0x00); 

	send_command(0x3A); 
	send_data_8bit(0x05);

	send_command(0x21); 

	send_command(0x2A);
	send_data_8bit(0x00);
	send_data_8bit(0x00);
	send_data_8bit(0x01);
	send_data_8bit(0x3F);

	send_command(0x2B);
	send_data_8bit(0x00);
	send_data_8bit(0x00);
	send_data_8bit(0x00);
	send_data_8bit(0xEF);

	send_command(0xB2);
	send_data_8bit(0x0C);
	send_data_8bit(0x0C);
	send_data_8bit(0x00);
	send_data_8bit(0x33);
	send_data_8bit(0x33);

	send_command(0xB7);
	send_data_8bit(0x35); 

	send_command(0xBB);
	send_data_8bit(0x1F);

	send_command(0xC0);
	send_data_8bit(0x2C);

	send_command(0xC2);
	send_data_8bit(0x01);

	send_command(0xC3);
	send_data_8bit(0x12);   

	send_command(0xC4);
	send_data_8bit(0x20);

	send_command(0xC6);
	send_data_8bit(0x0f); 

	send_command(0xD0);
	send_data_8bit(0xA4);
	send_data_8bit(0xA1);

	send_command(0xE0);
	send_data_8bit(0xD0);
	send_data_8bit(0x08);
	send_data_8bit(0x11);
	send_data_8bit(0x08);
	send_data_8bit(0x0C);
	send_data_8bit(0x15);
	send_data_8bit(0x39);
	send_data_8bit(0x33);
	send_data_8bit(0x50);
	send_data_8bit(0x36);
	send_data_8bit(0x13);
	send_data_8bit(0x14);
	send_data_8bit(0x29);
	send_data_8bit(0x2D);

	send_command(0xE1);
	send_data_8bit(0xD0);
	send_data_8bit(0x08);
	send_data_8bit(0x10);
	send_data_8bit(0x08);
	send_data_8bit(0x06);
	send_data_8bit(0x06);
	send_data_8bit(0x39);
	send_data_8bit(0x44);
	send_data_8bit(0x51);
	send_data_8bit(0x0B);
	send_data_8bit(0x16);
	send_data_8bit(0x14);
	send_data_8bit(0x2F);
	send_data_8bit(0x31);
	send_command(0x21);

	send_command(0x11);

	send_command(0x29);

	send_command(0x35);
}

void set_windows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend)
{
    //set the X coordinates
    send_command(0x2A);
    send_data_8bit(Xstart >>8);
    send_data_8bit(Xstart & 0xff);
	send_data_8bit((Xend - 1) >> 8);
    send_data_8bit((Xend-1) & 0xFF);

    //set the Y coordinates
    send_command(0x2B);
    send_data_8bit(Ystart >>8);
	send_data_8bit(Ystart & 0xff);
	send_data_8bit((Yend - 1) >> 8);
    send_data_8bit((Yend - 1) & 0xff);

    send_command(0X2C);
    // printf("%d %d\r\n",x,y);
}