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
    init_lcd(0);
    set_windows(0,0,HEIGHT_DISPLAY,WIDTH_DISPLAY);
    clear_buffer();
    draw_buffer();
    
    init_fire();
    uint32_t t=0;
    init_sin_lut();

    Mesh* cubeColored = createColoredCube(0x34c9);
    Mesh* cubeTextured = createTexturedCube(cube_texture,128);
    PointLight* lightWhite = createLight(50,50,50,10,0xffff);
    PointLight* lightRed = createLight(50,50,50,10,0xf800);
    PointLight* lightBlue = createLight(50,50,50,10,0x07e0);
    PointLight* lightGreen = createLight(50,50,50,10,0x001f);
    int x=0;
    int d=1;
    int colorNum=0;
    cubeTextured->transformations=addTransformation(cubeTextured->transformations,&cubeTextured->transformationsNum,0.0f,0.0f,0.0f,0);
    while (1)
    {
        float qt=t*0.05f;
        clear_buffer();
        draw_model(cubeTextured,lightWhite);
        cubeTextured->transformations[0].transformMatrix->x=qt;
        cubeTextured->transformations[0].transformMatrix->y=qt;
        // if(stage==0)
        // {
        //     x+=10*d;
        //     if(x>=80) d=-1;
        //     if(x<=0) d=1;
        //     draw_image(image1,240,240,x,0);
        // }
        // else if(stage==1)
        //     draw_model(cubeColored,lightWhite,t);
        //     // draw_pixel(10,10,0xaaaa);
        //     // draw_pixel(310,230,0xaaaa);
        //     // draw_pixel(310,10,0xaaaa);
        //     // draw_pixel(10,230,0xaaaa);
        // else if(stage==2)
        //     plasma(t);
        // else if(stage==3)
        // {
        //     draw_model(cubeTextured,lightWhite,t);
        // }
        // else if(stage==4)
        // {
        //     if(t%10)
        //     {
        //         colorNum++;
        //         if(colorNum>2)colorNum=0;    
        //     }
        //     if(colorNum==0)
        //         draw_model(cubeTextured,lightBlue,t);
        //     else if(colorNum==1)
        //         draw_model(cubeTextured,lightRed,t);
        //     else 
        //         draw_model(cubeTextured,lightGreen,t);
        // }
        // else
        // {
        //     clear_buffer();
        // }
        draw_buffer();
        t++;
        // if(t%30==0)
        //     stage++;
        //     if (stage>4)
        //         stage=0;
    }   
    freeModel(cubeTextured);
    freeModel(cubeColored);
    return 0;
}
