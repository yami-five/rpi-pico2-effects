#include "lcd.h"
#include "effects.h"
#include "DEV_Config.h"
#include "rasterizer.h"
#include "painter.h"
#include "models.h"
#include "gfx.h"

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
    if(DEV_Module_Init()!=0){
        return -1;
    }
    /* LCD Init */
    printf("2inch LCD demo...\r\n");
    init_lcd(0);
    set_windows(0,0,HEIGHT_DISPLAY,WIDTH_DISPLAY);
    clear_buffer();
    draw_buffer();
    
    init_fire();
    uint32_t t=0;
    init_sin_lut();

    Mesh* cube = createColoredCube(0x34c9);
    PointLight light = createLight(50,50,50,0,0);
    int x=0;
    int d=1;
    while (1)
    {
        if(t%30==0)
            // stage++;
            if (stage>3)
                stage=0;
        clear_buffer();
        if(stage==0)
        {
            draw_model(cube,&light,t);
            // draw_pixel(10,10,0xaaaa);
            // draw_pixel(310,230,0xaaaa);
            // draw_pixel(310,10,0xaaaa);
            // draw_pixel(10,230,0xaaaa);
        }
        else if(stage==1)
            draw_fire();
        else if(stage==2)
        {
            x+=10*d;
            if(x>=80) d=-1;
            if(x<=0) d=1;
            draw_image(image1,240,240,x,0);
        }
        else
            plasma(t);
        draw_buffer();
        t++;
    }   
    freeModel(cube);
    return 0;
}
