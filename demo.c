#include "lcd.h"
#include "effects.h"
#include "DEV_Config.h"
#include "rasterizer.h"
#include "painter.h"

#define FOCAL_LENGTH 90
#define WIDTH_DISPLAY 320
#define HEIGHT_DISPLAY 240
#define WIDTH_HALF 160
#define HEIGHT_HALF 120

#define HORIZONTAL 0
#define VERTICAL 1

int main(void)
{
    int stage=0;
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
    init_lcd(0);
    clear_buffer();
    set_windows(0,0,HEIGHT_DISPLAY,WIDTH_DISPLAY);
    draw_buffer();
    
    init_fire();
    uint32_t t=0;
    init_sin_lut();

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
    int x=0;
    int d=1;
    while (1)
    {
        if(t%100==0)
            stage++;
            if (stage>2)
                stage=0;
        clear_buffer();
        if(stage==0)
        {
            draw_image(image1,240,240,x,0);
            x+=10*d;
            if(x>=80) d=-1;
            if(x<=0) d=1;
            draw_model(&cube,t);
            // draw_pixel(10,10,0xaaaa);
            // draw_pixel(310,230,0xaaaa);
            // draw_pixel(310,10,0xaaaa);
            // draw_pixel(10,230,0xaaaa);
        }
        else if(stage==1)
            draw_fire();
        else
            plasma(t);
        draw_buffer();
        t++;
    }   

    return 0;
}
