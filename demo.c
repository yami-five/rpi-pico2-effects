#include "LCD_Test.h"
// #include "blink.h"
#include "LCD_2in.h"
#include "effects.h"
#include "DEV_Config.h"
#include "LCD_2in.c"

int main(void)
{
    //blink();
    // LCD_2in_test();
    DEV_Delay_ms(100);
    printf("LCD_2in_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }
    DEV_SET_PWM(50);
    /* LCD Init */
    printf("2inch LCD demo...\r\n");
    LCD_2IN_Init(HORIZONTAL);
    LCD_2IN_Clear(WHITE);
    
    //LCD_SetBacklight(1023);
    UDOUBLE Imagesize = LCD_2IN_HEIGHT*LCD_2IN_WIDTH*2;
    UWORD *BlackImage;
    if((BlackImage = (UWORD *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        exit(0);
    }
    // /*1.Create a new image cache named IMAGE_RGB and fill it with white*/
    Paint_NewImage((UBYTE *)BlackImage,LCD_2IN.WIDTH,LCD_2IN.HEIGHT, 90, WHITE);
    Paint_SetScale(65);
    Paint_Clear(WHITE);
    // /* GUI */
    printf("drawing...\r\n");
    // /*2.Drawing on the image*/

    init_fire();
    // uint32_t t=0;
    // uint32_t screen_size=320*240;
    init_sin_lut();
    // uint16_t color=0x0000;
    while (1)
    {
        draw_fire();
        // color++;
        // plasma(t);
        // t++;
    }   

    return 0;
}
