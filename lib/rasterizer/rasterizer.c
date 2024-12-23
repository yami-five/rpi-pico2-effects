#include "rasterizer.h"
#include <math.h>
#include "DEV_Config.h"
#include <stdint.h>
#include <stdlib.h>
#include "effects.h"
#include "painter.h"

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
#define PI 3.141592653589793
#define SHADING_ENABLED 0

// float fast_sqrt(float x)
// {
//     float y = x;
//     int i = *(int *)&y;
//     i = (i + 0x3f800000) >> 1;
//     y = *(float *)&i;
//     y = (y + x / y) * 0.5f;
//     return y;
// }

float vector3_length(int *vector)
{
    return fast_sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
}

void normalize_vector(int *vector3)
{
    float vectorLength = vector3_length(vector3);
    for (uint8_t i = 0; i < 3; i++)
        vector3[i] /= vectorLength;
}

void triangle_center(int *triangle, int *center)
{
    center[0] = (triangle[0] + triangle[3] + triangle[6]) / 3;
    center[1] = (triangle[1] + triangle[4] + triangle[7]) / 3;
    center[2] = (triangle[2] + triangle[5] + triangle[8]) / 3;
}

void rotate(int *x, int *y, uint32_t t)
{
    float c = cos(t);
    float s = sin(t);
    int temp_x = (int)(c * *x - s * *y);
    int temp_y = (int)(c * *x + s * *y);
    *x = temp_x;
    *y = temp_y;
}

int checkIfTriangleVisible(int *triangle)
{
    int e1x = triangle[2] - triangle[0];
    int e1y = triangle[3] - triangle[1];
    int e2x = triangle[4] - triangle[0];
    int e2y = triangle[5] - triangle[1];
    return (e1x * e2y - e1y * e2x) >= 0;
}

void rasterize(int y, int x0, int x1, int *triangle, Material *mat, int lightDistance)
{
    if (y < 0 || y >= 240)
        return;
    int yp2y = y - triangle[5];
    int n = y % 2;
    x0 += n;
    x1 += n;
    int q;
    if (x1 < x0)
    {
        q = x0;
        x0 = x1;
        x1 = q;
    }
    if (x1 < 0 || x0 >= 240)
        return;
    if (x0 < 0)
        x0 = 0;
    if (x1 >= 240)
        x1 = 239;
    x0 = x0 >> 1;
    x1 = x1 >> 1;
    for (int x = x0; x<x1; x++)
    {
        uint16_t color = mat->diffuse;
        draw_pixel(x,y,color);
    }
}

void tri(int *triangle, Material *mat, int lightDistance)
{
    // x0,y0=triangle[0],triangle[1]
    // x1,y1=triangle[2],triangle[3]
    // x2,y2=triangle[4],triangle[5]
    int x, y;
    if (triangle[1] > triangle[3])
    {
        y = triangle[1];
        triangle[1] = triangle[3];
        triangle[3] = y;
        x = triangle[0];
        triangle[0] = triangle[2];
        triangle[2] = x;
    }
    if (triangle[1] > triangle[5])
    {
        y = triangle[1];
        triangle[1] = triangle[5];
        triangle[5] = y;
        x = triangle[0];
        triangle[0] = triangle[4];
        triangle[4] = x;
    }
    if (triangle[3] > triangle[5])
    {
        y = triangle[3];
        triangle[3] = triangle[5];
        triangle[5] = y;
        x = triangle[2];
        triangle[2] = triangle[4];
        triangle[4] = x;
    }
    if (triangle[5] < 0 || triangle[1] > 239)
        return;
    y = triangle[1];
    x = triangle[0];
    int xx = triangle[0];
    int dx01 = triangle[2] - triangle[0];
    int dy01 = triangle[3] - triangle[1];

    int dx02 = triangle[4] - triangle[0];
    int dy02 = triangle[5] - triangle[1];

    int dx12 = triangle[4] - triangle[2];
    int dy12 = triangle[5] - triangle[3];

    int q2 = 0;
    int xxd = 1;

    if (triangle[4] < triangle[0])
        xxd = -1;

    if (triangle[1] < triangle[3])
    {
        int q = 0;
        int xd = 1;
        if (triangle[2] < triangle[0])
            xd = -1;
        while (y <= triangle[3])
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

    if (triangle[3] < triangle[5])
    {
        int q = 0;
        int x = triangle[2];
        int xd = 1;
        if (triangle[4] < triangle[2])
            xd = -1;

        int test=triangle[5];
        while (y <= triangle[5] && y < 240)
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

void draw_model(Mesh *mesh, uint32_t t)
{
    uint16_t verticesCounter = mesh->verticesCounter;
    int verticesModified[verticesCounter * 3];
    int verticesOnScreen[verticesCounter * 2];
    uint8_t vsc=0;
    for (uint16_t i = 0; i < verticesCounter * 3; i += 3)
    {
        // calculates vertex coords in 3D space
        int x = (mesh->vertices)[i];
        int y = mesh->vertices[i + 1];
        int z = mesh->vertices[i + 2];
        rotate(&x, &z, t);
        rotate(&y, &z, t);
        verticesModified[i] = x;
        verticesModified[i + 1] = y;
        verticesModified[i + 2] = z;
        // calculates vertex coords on the screen
        z += 5;
        x = (((x * FOCAL_LENGTH << 10) / z) >> 10) + HEIGHT_HALF;
        y = (((y * FOCAL_LENGTH << 10) / z) >> 10) + HEIGHT_HALF;
        verticesOnScreen[vsc] = x;
        verticesOnScreen[vsc + 1] = y;
        vsc+=2;
    }
    // flat shading

    for (uint16_t i = 0; i<mesh->facesCounter * 3; i += 3)
    {
        uint16_t a = mesh->faces[i];
        uint16_t b = mesh->faces[i + 1];
        uint16_t c = mesh->faces[i + 2];
        int triangleOnScreen[6] =
            {
                verticesOnScreen[a * 2],
                verticesOnScreen[a * 2 + 1],
                verticesOnScreen[b * 2],
                verticesOnScreen[b * 2 + 1],
                verticesOnScreen[c * 2],
                verticesOnScreen[c * 2 + 1]};
        if (!checkIfTriangleVisible(triangleOnScreen))
            continue;
        // normal vector
        int lightDistance=0;
        if (SHADING_ENABLED)
        {
            int xab = (verticesModified[b * 3] << 10) - (verticesModified[a * 3] << 10);
            int yab = (verticesModified[b * 3 + 1] << 10) - (verticesModified[a * 3 + 1] << 10);
            int zab = (verticesModified[b * 3 + 2] << 10) - (verticesModified[a * 3 + 2] << 10);
            int xac = (verticesModified[c * 3] << 10) - (verticesModified[a * 3] << 10);
            int yac = (verticesModified[c * 3 + 1] << 10) - (verticesModified[a * 3 + 1] << 10);
            int zac = (verticesModified[c * 3 + 2] << 10) - (verticesModified[a * 3 + 2] << 10);
            int normalVector[3] =
                {
                    (yab * zac - zab * yac) >> 10,
                    (zab * xac - xab * zac) >> 10,
                    (xab * yac - yab * xac) >> 10};
            normalize_vector(normalVector);
            int normalVectorLength = vector3_length(normalVector);
            // light direction
            int triangle[9] =
                {
                    verticesModified[a * 3],
                    verticesModified[a * 3 + 1],
                    verticesModified[a * 3 + 2],
                    verticesModified[b * 3],
                    verticesModified[b * 3 + 1],
                    verticesModified[b * 3 + 2],
                    verticesModified[c * 3],
                    verticesModified[c * 3 + 1],
                    verticesModified[c * 3 + 2],
                };
            int center[3];
            triangle_center(triangle, center);
            int lightDirection[3] =
                {
                    50 - center[0],
                    50 - center[1],
                    50 - center[2]};
            int lightLength = vector3_length(lightDirection);
            // light distance
            int vector[3] =
                {
                    lightDirection[0] - normalVector[0],
                    lightDirection[1] - normalVector[1],
                    lightDirection[2] - normalVector[2]};
            int lightDirectionMinusNormalVector = vector3_length(vector);
            int x = normalVectorLength * normalVectorLength + lightLength * lightLength - lightDirectionMinusNormalVector * lightDirectionMinusNormalVector;
            int y = (lightLength * normalVectorLength) * 2;
            lightDistance = x / y;
            if (lightDistance < 0)
                lightDistance = 0;
        }

        tri(triangleOnScreen, mesh->mat, lightDistance);
    }
}
