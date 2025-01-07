#include "lcd.h"
#include "effects.h"
#include "DEV_Config.h"
#include "rasterizer.h"
#include "painter.h"
#include "models.h"
#include "gfx.h"
#include <stdlib.h>

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
    
    // init_fire();
    uint32_t t=0;
    // init_sin_lut();

    Mesh* cubeColored = createColoredCube(0xffff);
    Mesh* cubeTextured1 = createTexturedCube(wall_texture,128);
    Mesh* cubeTextured2 = createTexturedCube(cube_texture,128);
    Mesh* cubeTextured3 = createTexturedCube(cube_texture,128);
    Mesh* cubeTextured4 = createTexturedCube(wall_texture,128);
    // Mesh* wallTextured1 = createTexturedPlane(wall_texture,128);
    // Mesh* wallTextured2 = createTexturedPlane(wall_texture,128);
    PointLight* lightWhite = createLight(0,0,50,1.0f,0xffff);
    PointLight* lightRed = createLight(0,0,50,1.0f,0xf800);
    PointLight* lightBlue = createLight(0,0,50,1.0f,0x07e0);
    PointLight* lightGreen = createLight(0,0,50,1.0f,0x001f);
    int x=0;
    int d=1;
    int colorNum=0;
    cubeTextured1->transformations=addTransformation(cubeTextured1->transformations,&cubeTextured1->transformationsNum,0.0f,0.0f,0.0f,0);
    cubeTextured2->transformations=addTransformation(cubeTextured2->transformations,&cubeTextured2->transformationsNum,0.0f,0.0f,0.0f,0);
    cubeTextured3->transformations=addTransformation(cubeTextured3->transformations,&cubeTextured3->transformationsNum,0.0f,0.0f,0.0f,0);
    cubeTextured4->transformations=addTransformation(cubeTextured4->transformations,&cubeTextured4->transformationsNum,0.0f,0.0f,0.0f,0);

    cubeTextured1->transformations=addTransformation(cubeTextured1->transformations,&cubeTextured1->transformationsNum,3.0f,2.0f,0.0f,1);
    cubeTextured2->transformations=addTransformation(cubeTextured2->transformations,&cubeTextured2->transformationsNum,3.0f,-2.0f,0.0f,1);
    cubeTextured3->transformations=addTransformation(cubeTextured3->transformations,&cubeTextured3->transformationsNum,-3.0f,2.0f,0.0f,1);
    cubeTextured4->transformations=addTransformation(cubeTextured4->transformations,&cubeTextured4->transformationsNum,-3.0f,-2.0f,0.0f,1);
    // wallTextured1->transformations=addTransformation(wallTextured1->transformations,&wallTextured1->transformationsNum,0.875f,0.1f,0.0f,0);
    // wallTextured1->transformations=addTransformation(wallTextured1->transformations,&wallTextured1->transformationsNum,5.0f,5.0f,0.0f,2);
    // wallTextured1->transformations=addTransformation(wallTextured1->transformations,&wallTextured1->transformationsNum,0.0f,2.5f,0.0f,1);

    // wallTextured2->transformations=addTransformation(wallTextured2->transformations,&wallTextured2->transformationsNum,0.875f,0.1f,0.0f,0);
    // wallTextured2->transformations=addTransformation(wallTextured2->transformations,&wallTextured2->transformationsNum,5.0f,5.0f,0.0f,2);
    // wallTextured2->transformations=addTransformation(wallTextured2->transformations,&wallTextured2->transformationsNum,-5.0f,2.5f,0.0f,1);
    float lightIntensity[6]={1.0f,0.75f,0.3f,0.1f,0.5f,0.75f};
    while (1)
    {
        lightWhite->intensity=lightIntensity[t%6];
        lightRed->intensity=lightIntensity[(t+1)%6];
        lightGreen->intensity=lightIntensity[(t+2)%6];
        lightBlue->intensity=lightIntensity[(t+3)%6];
        float qt=t*0.01f;
        clear_buffer();
        // draw_model(wallTextured1,lightWhite);
        // draw_model(wallTextured2,lightWhite);
        cubeTextured1->transformations[0].transformVector->x=qt+0.05f;
        cubeTextured1->transformations[0].transformVector->y=qt+0.05f;
        cubeTextured1->transformations[0].transformVector->z=qt+0.05f;
        draw_model(cubeTextured1,lightWhite);

        cubeTextured2->transformations[0].transformVector->x=qt+0.2f;
        cubeTextured2->transformations[0].transformVector->y=qt+0.2f;;
        cubeTextured2->transformations[0].transformVector->z=qt+0.2f;;
        draw_model(cubeTextured2,lightRed);

        cubeTextured3->transformations[0].transformVector->x=qt;
        cubeTextured3->transformations[0].transformVector->z=qt;
        draw_model(cubeTextured3,lightBlue);

        cubeTextured4->transformations[0].transformVector->x=qt+1.0f;
        cubeTextured4->transformations[0].transformVector->y=qt+1.0f;
        cubeTextured4->transformations[0].transformVector->z=qt+1.0f;
        draw_model(cubeTextured4,lightGreen);
        // wallTextured1->transformations[0].transformVector->x=qt;
        
        draw_buffer();
        t++;
        malloc_stats();
    }   
    freeModel(cubeTextured1);
    freeModel(cubeColored);
    return 0;
}
