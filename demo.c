#include "LCD_Test.h"
// #include "blink.h"
#include "LCD_2in.h"
#include "effects.h"
#include "DEV_Config.h"
#include "LCD_2in.c"
#include "rasterizer.h"
#include "painter.h"

#define FOCAL_LENGTH 90
#define WIDTH_DISPLAY 320
#define HEIGHT_DISPLAY 240
#define WIDTH_HALF 160
#define HEIGHT_HALF 120

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

    // init_fire();
    uint32_t t=0;
    // uint32_t screen_size=320*240;
    init_sin_lut();
    // uint16_t color=0x0000;

    Material material=
    {
        0xabcd,
        {0}
    };
    int vertices[24]={1.3,1.3,-1.3,1.3,-1.3,-1.3,1.3,1.3,1.3,1.3,-1.3,1.3,-1.3,1.3,-1.3,-1.3,-1.3,-1.3,-1.3,1.3,1.3,-1.3,-1.3,1.3};
    uint16_t faces[36]={0,2,4,3,7,2,7,5,6,5,7,1,1,3,0,5,1,4,2,6,4,7,6,2,5,4,6,7,3,1,3,2,0,1,0,4};
    uint16_t textureCoords[8]={1,0,0,1,0,0,1,1};
    uint16_t uv[36]={2,1,0,2,1,0,1,3,2,0,2,3,2,1,0,1,3,2,1,3,0,1,3,0,3,0,2,2,1,3,1,3,0,3,0,2};
    Mesh cube={
        8, //verticesCounter
        12,//facesCounter
        vertices,//vertices
        faces,//faces
        textureCoords,//textureCoords
        uv,//uv
        &material//mat
    };
    
    while (1)
    {
        clear_buffer();
        draw_model(&cube,t);
        draw_pixel(10,10,0xaaaa);
        draw_pixel(310,230,0xaaaa);
        draw_pixel(310,10,0xaaaa);
        draw_pixel(10,230,0xaaaa);

        draw_pixel(160,120,0xaaaa);

        for (int i=0; i<24;i+=3)
        {
            int z = vertices[i+2]+5;
            int x = vertices[i] * FOCAL_LENGTH / z + WIDTH_HALF;
            int y = vertices[i+1] * FOCAL_LENGTH / z + HEIGHT_HALF;
            draw_pixel(x,y,0xaaaa);
        }
        // draw_pixel(344,144,0xaaaa);
        // draw_pixel(344,96,0xaaaa);

        // draw_pixel(336,136,0xaaaa);
        // draw_pixel(336,104,0xaaaa);
        
        // draw_pixel(296,144,0xaaaa);
        // draw_pixel(296,96,0xaaaa);

        // draw_pixel(304,136,0xaaaa);
        // draw_pixel(304,104,0xaaaa);
        draw_buffer();
        // draw_fire();
        // color++;
        // plasma(t);
        t++;
    }   

    return 0;
}
