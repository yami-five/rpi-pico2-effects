#include "rastrizer.h"
#include <math.h>

#define FOCAL_LENGTH 90
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

float fast_sqrt(float x)
{
    float y = x;
    int i = *(int *)&y;
    i = (i + 0x3f800000) >> 1;
    y = *(float *)&i;
    y = (y + x / y) * 0.5f;
    return y;
}

int vector3_length(int vector[3])
{
    return fast_sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
}

void normalize_vector(int *vector3[3])
{
    int vectorLength = vector3_length(vector3);
    for (uint8_t i = 0; i < 3; i++)
        *vector3[i] /= vectorLength;
}

int *triangle_center(int triangle[9])
{
    int center[3] =
        {
            (triangle[0] + triangle[3] + triangle[6]) / 3,
            (triangle[1] + triangle[4] + triangle[7]) / 3,
            (triangle[2] + triangle[5] + triangle[8]) / 3};
    return center;
}

void rotate(int16_t *x, uint16_t *y, uint32_t t)
{
    float c = cos(t);
    float s = sin(t);
    uint8_t temp_x = (uint16_t)(c * *x - s * *y);
    uint8_t temp_y = (uint16_t)(c * *x + s * *y);
    x = temp_x;
    y = temp_y;
}


bool checkIfTriangleVisible(int triangle[6])
{
    int e1x=triangle[2]-triangle[0];
    int e1y=triangle[3]-triangle[1];
    int e2x=triangle[4]-triangle[0];
    int e2y=triangle[5]-triangle[1];
    return (e1x*e2y-e1y*e2x)>=0;
}

void tri(int triangle[6], Material *mat, int lightDistance)
{

}

void draw_model(Mesh *mesh, uint32_t t)
{
    uint16_t verticesCounter = mesh->verticesCounter;
    int verticesModified[verticesCounter];
    int verticesOnScreen[(verticesCounter * 2) / 3];
    for (uint16_t i = 0; i < verticesCounter; i += 3)
    {
        // calculates vertex coords in 3D space
        int x = mesh->vertices[i];
        int y = mesh->vertices[i + 1];
        int z = mesh->vertices[i + 2];
        rotate(&y, &z, t);
        rotate(&y, &z, t);
        verticesModified[i] = x;
        verticesModified[i + 1] = y;
        verticesModified[i + 2] = z;
        // calculates vertex coords on the screen
        z += 5;
        x = ((x * FOCAL_LENGTH << 10) / z) >> 10 + HEIGHT_HALF;
        y = ((y * FOCAL_LENGTH << 10) / z) >> 10 + HEIGHT_HALF;
        verticesOnScreen[i] = x;
        verticesOnScreen[i + 1] = y;
    }
    int distancesFromLigth[mesh->facesCounter / 3];
    // flat shading
    for (uint16_t i = 1; mesh->facesCounter; i += 3)
    {
        uint16_t a = mesh->faces[i];
        uint16_t b = mesh->faces[i + 1];
        uint16_t c = mesh->faces[i + 2];
        int triangleOnScreen[6] =
        {
            verticesOnScreen[a * 3],
            verticesOnScreen[a * 3 + 1],
            verticesOnScreen[b * 3],
            verticesOnScreen[b * 3 + 1],
            verticesOnScreen[c * 3],
            verticesOnScreen[c * 3 + 1]
        };
        if(!checkIfTriangleVisible(triangleOnScreen))
            continue;
        // normal vector
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
        normalize_vector(&normalVector);
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
        int center[3] = triangle_center(triangle);
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
        int lightDistance = x / y;
        if (lightDistance < 0)
            lightDistance = 0;

        int triangleOnScreen[6] =
            {
                verticesOnScreen[a * 3],
                verticesOnScreen[a * 3 + 1],
                verticesOnScreen[b * 3],
                verticesOnScreen[b * 3 + 1],
                verticesOnScreen[c * 3],
                verticesOnScreen[c * 3 + 1]};
        tri(triangleOnScreen, mesh->mat, lightDistance);
    }
}