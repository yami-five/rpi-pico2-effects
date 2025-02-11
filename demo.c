#include "lcd.h"
#include "effects.h"
#include "DEV_Config.h"
#include "rasterizer.h"
#include "painter.h"
#include "models.h"
#include "gfx.h"
#include "fpa.h"
#include "sd_reader.h"

#define FOCAL_LENGTH 90
#define WIDTH_DISPLAY 320
#define HEIGHT_DISPLAY 240
#define WIDTH_HALF 160
#define HEIGHT_HALF 120

#define HORIZONTAL 0
#define VERTICAL 1

int main(void)
{
    initSinCos();
    // int stage=0;
    if (DEV_Module_Init() != 0)
    {
        return -1;
    }
    /* LCD Init */
    init_lcd(0);
    sd_init();
    set_windows(0, 0, HEIGHT_DISPLAY, WIDTH_DISPLAY);
    clear_buffer();
    draw_buffer();

    // init_fire();
    uint32_t t = 0;
    // init_sin_lut();

    Mesh *cubeColored = createColoredCube(0xffff);
    Mesh *cubeTextured1 = createTexturedCube(wall_texture, 32);
    Mesh *cubeTextured2 = createTexturedCube(box_texture, 32);
    Mesh *cubeTextured3 = createTexturedCube(box_texture, 32);
    Mesh *cubeTextured4 = createTexturedCube(wall_texture, 32);
    PointLight *lightWhite = createLight(floatToFixed(0.0f), floatToFixed(0.0f), floatToFixed(50.0f), floatToFixed(1.0f), 0xffff);
    PointLight *lightRed = createLight(floatToFixed(0.0f), floatToFixed(0.0f), floatToFixed(50.0f), floatToFixed(1.0f), 0xf800);
    PointLight *lightBlue = createLight(floatToFixed(0.0f), floatToFixed(0.0f), floatToFixed(50.0f), floatToFixed(1.0f), 0x07e0);
    PointLight *lightGreen = createLight(floatToFixed(0.0f), floatToFixed(0.0f), floatToFixed(50.0f), floatToFixed(1.0f), 0x001f);
    int x = 0;
    int d = 1;
    int colorNum = 0;
    cubeTextured1->transformations = addTransformation(cubeTextured1->transformations, &cubeTextured1->transformationsNum, 0, 0, 0, 0);
    cubeTextured2->transformations = addTransformation(cubeTextured2->transformations, &cubeTextured2->transformationsNum, 0, 0, 0, 0);
    cubeTextured3->transformations = addTransformation(cubeTextured3->transformations, &cubeTextured3->transformationsNum, 0, 0, 0, 0);
    cubeTextured4->transformations = addTransformation(cubeTextured4->transformations, &cubeTextured4->transformationsNum, 0, 0, 0, 0);

    cubeTextured1->transformations = addTransformation(cubeTextured1->transformations, &cubeTextured1->transformationsNum, floatToFixed(3.0f), floatToFixed(2.0f), 0, 1);
    cubeTextured2->transformations = addTransformation(cubeTextured2->transformations, &cubeTextured2->transformationsNum, floatToFixed(3.0f), floatToFixed(-2.0f), 0, 1);
    cubeTextured3->transformations = addTransformation(cubeTextured3->transformations, &cubeTextured3->transformationsNum, floatToFixed(-3.0f), floatToFixed(2.0f), 0, 1);
    cubeTextured4->transformations = addTransformation(cubeTextured4->transformations, &cubeTextured4->transformationsNum, floatToFixed(-3.0f), floatToFixed(-2.0f), 0, 1);
    int32_t lightIntensity[8] = {floatToFixed(1.0f), floatToFixed(0.75f), floatToFixed(0.25f), floatToFixed(0.0f), floatToFixed(0.25f), floatToFixed(0.5f), floatToFixed(0.75f), floatToFixed(1.0f)};
    int32_t t_factor=floatToFixed(0.1f);
    int fixed_005=floatToFixed(0.05f);
    int fixed_02=floatToFixed(0.2f);
    while (1)
    {
        lightWhite->intensity = lightRed->intensity = lightGreen->intensity = lightBlue->intensity = lightIntensity[t % 8];
        int32_t qt = t * t_factor;
        clear_buffer();
        cubeTextured1->transformations[0].transformVector->x =
            cubeTextured1->transformations[0].transformVector->y =
                cubeTextured1->transformations[0].transformVector->z = qt + fixed_005;
        draw_model(cubeTextured1, lightWhite);

        cubeTextured2->transformations[0].transformVector->x =
            cubeTextured2->transformations[0].transformVector->y =
                cubeTextured2->transformations[0].transformVector->z = qt + fixed_02;
        draw_model(cubeTextured2, lightRed);

        cubeTextured3->transformations[0].transformVector->x =
            cubeTextured3->transformations[0].transformVector->y =
                cubeTextured3->transformations[0].transformVector->z = qt;
        draw_model(cubeTextured3, lightBlue);

        cubeTextured4->transformations[0].transformVector->x =
            cubeTextured4->transformations[0].transformVector->y =
                cubeTextured4->transformations[0].transformVector->z = qt + SCALE_FACTOR;
        draw_model(cubeTextured4, lightGreen);
        // wallTextured1->transformations[0].transformVector->x=qt;

        draw_buffer();
        t++;
    }
    freeModel(cubeTextured1);
    freeModel(cubeColored);
    return 0;
}
