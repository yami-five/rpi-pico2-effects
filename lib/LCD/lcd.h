#include <stdint.h>

void send_command(uint8_t Reg);
void send_data_8bit(uint8_t Data);
void send_data_16bit(uint16_t Data);
void set_attributes(uint8_t Scan_dir);
void lcd_reset();
void init_reg();
void init_lcd(uint8_t Scan_dir);
void set_windows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend);