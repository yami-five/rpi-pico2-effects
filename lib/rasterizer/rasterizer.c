
#include "DEV_Config.h"
#include "effects.h"
#include "models.h"
#include "painter.h"
#include "rasterizer.h"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#define FOCAL_LENGTH 96
#define WIDTH_DISPLAY 320
#define HEIGHT_DISPLAY 240
#define WIDTH_DOUBLED 640
#define HEIGHT_DOUBLED 480
#define ARRAY_SIZE 153600
#define WIDTH_HALF 160
#define HEIGHT_HALF 120
#define FIRE_FLOOR_ADR 76480
#define SCALE_FACTOR 10000
#define PI 3.141592653589793f
#define PI2 6.283185307179586f

#define SHADING_ENABLED 1

PointLight createLight(int x, int y, int z, uint8_t intensity, uint16_t color)
{
    PointLight light=
    {
        {
            x,y,z
        },
        0,
        0
    };
    return light;
}

float vector3_length(float *vector)
{
    return sqrtf(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
}

void normalize_vector(float *vector3)
{
    float vectorLength = vector3_length(vector3);
    for (uint8_t i = 0; i < 3; i++)
        vector3[i] /= vectorLength;
}

void triangle_center(Triangle3D *triangle, float *center)
{
    center[0] = (triangle->a.x + triangle->b.x + triangle->c.x) / 3.0f;
    center[1] = (triangle->a.y + triangle->b.y + triangle->c.y) / 3.0f;
    center[2] = (triangle->c.z + triangle->b.z + triangle->c.z) / 3.0f;
}

void rotate(float *x, float *y, float qt)
{
    float qt_rad = qt * PI2;
    float c = cos(qt_rad);
    float s = sin(qt_rad);
    float temp_x = (c * *x - s * *y);
    float temp_y = (s * *x + c * *y);
    *x = temp_x;
    *y = temp_y;
}

void inf(float *x, float *y, float qt)
{
    float qt_rad = qt * PI2;
    *x += 2.0f * (cos(qt_rad));
    *y += 2.0f * cos(qt_rad) * sin(qt_rad);
}

int checkIfTriangleVisible(Triangle2D *triangle)
{
    int e1x = triangle->b.x - triangle->a.x;
    int e1y = triangle->b.y - triangle->a.y;
    int e2x = triangle->c.x - triangle->a.x;
    int e2y = triangle->c.y - triangle->a.y;
    return (e1x * e2y - e1y * e2x) >= 0;
}
 
void lightedColor(uint16_t *color, float lightDistance)
{
    lightDistance*=2;
    if(lightDistance!=1)
    {
        uint8_t r = (*color>>11)&0x1f;
        uint8_t g = (*color>>5)&0x1f;
        uint8_t b = *color&0x1f;   

        r = (uint8_t)(r * lightDistance + (1.0f - lightDistance) * (31 - r));
        g = (uint8_t)(g * lightDistance + (1.0f - lightDistance) * (63 - g));
        b = (uint8_t)(b * lightDistance + (1.0f - lightDistance) * (31 - b));   
        *color=(r<<11)|(g<<5)|b;
    }
}

void rasterize(int y, int x0, int x1, Triangle2D *triangle, Material *mat, float lightDistance)
{
    if (y < 0 || y >= HEIGHT_DISPLAY)
        return;
    int yp2y = y - triangle->c.y;
    int n = (y % 2) / 2;
    x0 += n;
    x1 += n;
    int q;
    if (x1 < x0)
    {
        q = x0;
        x0 = x1;
        x1 = q;
    }
    if (x1 < 0 || x0 >= WIDTH_DISPLAY)
        return;
    if (x0 < 0)
        x0 = 0;
    if (x1 > WIDTH_DISPLAY)
        x1 = WIDTH_DISPLAY;
    // x0 = x0 >> 1;
    // x1 = x1 >> 1;
    for (int x = x0; x < x1; x++)
    {
        uint16_t color=0;
        if (mat->textureSize==0)
            color = mat->diffuse;
        if (SHADING_ENABLED)
            lightedColor(&color, lightDistance);
        draw_pixel(x, y, color);
    }
}

void tri(Triangle2D *triangle, Material *mat, float lightDistance)
{
    // x0,y0=triangle->a.x,triangle->a.y
    // x1,y1=triangle->b.x,triangle->b.y
    // x2,y2=triangle->c.x,triangle->c.y
    int x, y;
    if (triangle->a.y > triangle->b.y)
    {
        y = triangle->a.y;
        triangle->a.y = triangle->b.y;
        triangle->b.y = y;
        x = triangle->a.x;
        triangle->a.x = triangle->b.x;
        triangle->b.x = x;
    }
    if (triangle->a.y > triangle->c.y)
    {
        y = triangle->a.y;
        triangle->a.y = triangle->c.y;
        triangle->c.y = y;
        x = triangle->a.x;
        triangle->a.x = triangle->c.x;
        triangle->c.x = x;
    }
    if (triangle->b.y > triangle->c.y)
    {
        y = triangle->b.y;
        triangle->b.y = triangle->c.y;
        triangle->c.y = y;
        x = triangle->b.x;
        triangle->b.x = triangle->c.x;
        triangle->c.x = x;
    }
    if (triangle->c.y < 0 || triangle->a.y > HEIGHT_DISPLAY)
        return;
    y = triangle->a.y;
    int xx = x = triangle->a.x;

    int dx01 = triangle->b.x - triangle->a.x;
    int dy01 = triangle->b.y - triangle->a.y;

    int dx02 = triangle->c.x - triangle->a.x;
    int dy02 = triangle->c.y - triangle->a.y;

    int dx12 = triangle->c.x - triangle->b.x;
    int dy12 = triangle->c.y - triangle->b.y;

    int q2 = 0;
    int xxd = 1;

    if (triangle->c.x < triangle->a.x)
        xxd = -1;

    if (triangle->a.y < triangle->b.y)
    {
        int q = 0;
        int xd = 1;
        if (triangle->b.x < triangle->a.x)
            xd = -1;
        while (y <= triangle->b.y)
        {
            rasterize(y, x, xx, triangle, mat, lightDistance);
            y += 1;
            q += dx01;
            q2 += dx02;
            while (xd * q >= dy01)
            {
                q -= xd * dy01;
                x += xd;
            }
            while (xxd * q2 >= dy02)
            {
                q2 -= xxd * dy02;
                xx += xxd;
            }
        }
    }

    if (triangle->b.y < triangle->c.y)
    {
        int q = 0;
        int x = triangle->b.x;
        int xd = 1;
        if (triangle->c.x < triangle->b.x)
            xd = -1;

        int test = triangle->c.y;
        while (y <= triangle->c.y && y < HEIGHT_DISPLAY)
        {
            rasterize(y, x, xx, triangle, mat, lightDistance);
            y += 1;
            q += dx12;
            q2 += dx02;
            while (xd * q > dy12)
            {
                q -= xd * dy12;
                x += xd;
            }
            while (xxd * q2 > dy02)
            {
                q2 -= xxd * dy02;
                xx += xxd;
            }
        }
    }
}

void draw_model(Mesh *mesh, PointLight *pLight, uint32_t t)
{
    float qt = t * 0.01f;
    uint16_t verticesCounter = mesh->verticesCounter;
    float verticesModified[verticesCounter * 3];
    int verticesOnScreen[verticesCounter * 2];
    uint8_t vsc = 0;
    for (uint16_t i = 0; i < verticesCounter * 3; i += 3)
    {
        // calculates vertex coords in 3D space
        float x = mesh->vertices[i];
        float y = mesh->vertices[i + 1];
        float z = mesh->vertices[i + 2];
        rotate(&x, &z, qt);
        rotate(&y, &z, qt);
        inf(&x, &y, qt);
        verticesModified[i] = x;
        verticesModified[i + 1] = y;
        verticesModified[i + 2] = z;
        // calculates vertex coords on the screen
        z += 5.0f;
        x = x * FOCAL_LENGTH / z + WIDTH_HALF;
        y = y * FOCAL_LENGTH / z + HEIGHT_HALF;
        verticesOnScreen[vsc] = (int)x;
        verticesOnScreen[vsc + 1] = (int)y;
        vsc += 2;
    }
    // flat shading

    // for (uint16_t i = 6; i<12; i += 3)
    for (uint16_t i = 0; i < mesh->facesCounter * 3; i += 3)
    {
        uint16_t a = mesh->faces[i];
        uint16_t b = mesh->faces[i + 1];
        uint16_t c = mesh->faces[i + 2];
        Triangle2D triangle =
            {
                {verticesOnScreen[a * 2],
                 verticesOnScreen[a * 2 + 1]},
                {verticesOnScreen[b * 2],
                 verticesOnScreen[b * 2 + 1]},
                {verticesOnScreen[c * 2],
                 verticesOnScreen[c * 2 + 1]}};
        if (!checkIfTriangleVisible(&triangle))
            continue;
        // normal vector
        float lightDistance = 0.0f;
        if (SHADING_ENABLED)
        {
            float xab = verticesModified[b * 3] - verticesModified[a * 3];
            float yab = verticesModified[b * 3 + 1] - verticesModified[a * 3 + 1];
            float zab = verticesModified[b * 3 + 2] - verticesModified[a * 3 + 2];
            float xac = verticesModified[c * 3] - verticesModified[a * 3];
            float yac = verticesModified[c * 3 + 1] - verticesModified[a * 3 + 1];
            float zac = verticesModified[c * 3 + 2] - verticesModified[a * 3 + 2];
            float normalVector[3] =
                {
                    (yab * zac - zab * yac),
                    (zab * xac - xab * zac),
                    (xab * yac - yab * xac)};
            normalize_vector(normalVector);
            float normalVectorLength = vector3_length(normalVector);
            // light direction
            Triangle3D triangle3D = {
                {verticesModified[a * 3],
                 verticesModified[a * 3 + 1],
                 verticesModified[a * 3 + 2]},
                {verticesModified[b * 3],
                 verticesModified[b * 3 + 1],
                 verticesModified[b * 3 + 2]},
                {verticesModified[c * 3],
                 verticesModified[c * 3 + 1],
                 verticesModified[c * 3 + 2]}};
            float center[3];
            triangle_center(&triangle3D, center);
            float lightDirection[3] =
                {
                    pLight->position.x - center[0],
                    pLight->position.y - center[1],
                    pLight->position.z - center[2]};
            float lightLength = vector3_length(lightDirection);
            // light distance
            float vector[3] =
                {
                    lightDirection[0] - normalVector[0],
                    lightDirection[1] - normalVector[1],
                    lightDirection[2] - normalVector[2]};
            float lightDirectionMinusNormalVector = vector3_length(vector);
            float x = normalVectorLength * normalVectorLength + lightLength * lightLength - lightDirectionMinusNormalVector * lightDirectionMinusNormalVector;
            float y = (lightLength * normalVectorLength) * 2;
            lightDistance = x / y;
            if (lightDistance < 0.0f)
                lightDistance = 0.0f;
            if (lightDistance > 1.0f)
                lightDistance = 1.0f;
        }

        tri(&triangle, mesh->mat, lightDistance);
    }
}
