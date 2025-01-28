
#include "DEV_Config.h"
#include "effects.h"
#include "models.h"
#include "painter.h"
#include "rasterizer.h"
#include "fpa.h"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#define FOCAL_LENGTH 90.0f
#define WIDTH_DISPLAY 320
#define HEIGHT_DISPLAY 240
#define WIDTH_DOUBLED 640
#define HEIGHT_DOUBLED 480
#define ARRAY_SIZE 153600
#define WIDTH_HALF 160
#define HEIGHT_HALF 120
#define FIRE_FLOOR_ADR 76480
#define TRIANGLE_CENTER_DIVIDER 3 * SCALE_FACTOR

#define SHADING_ENABLED 1

PointLight *createLight(int x, int y, int z, uint8_t intensity, uint16_t color)
{
    PointLight *light = (PointLight *)malloc(sizeof(PointLight));
    light->position.x = x;
    light->position.y = y;
    light->position.z = z;
    light->intensity = intensity;
    light->color = color;
    return light;
}

int32_t vector3_length(int32_t *vector)
{
    return fastSqrt(fixedPow(vector[0]) + fixedPow(vector[1]) + fixedPow(vector[2]));
}

void normalize_vector(int32_t *vector3)
{
    int32_t vectorLength = vector3_length(vector3);
    for (uint8_t i = 0; i < 3; i++)
        vector3[i] = fixedDiv(vector3[i], vectorLength);
}

void triangle_center(Triangle3D *triangle, int32_t *center)
{
    center[0] = fixedDiv((triangle->a.x + triangle->b.x + triangle->c.x), TRIANGLE_CENTER_DIVIDER);
    center[1] = fixedDiv((triangle->a.y + triangle->b.y + triangle->c.y), TRIANGLE_CENTER_DIVIDER);
    center[2] = fixedDiv((triangle->c.z + triangle->b.z + triangle->c.z), TRIANGLE_CENTER_DIVIDER);
}

void transform(int32_t *x, int32_t *y, int32_t *z, TransformInfo *transformInfo)
{
    if (transformInfo->transformType == 0)
        rotate(x, y, z, transformInfo->transformVector);
    if (transformInfo->transformType == 1)
        translate(x, y, z, transformInfo->transformVector);
    if (transformInfo->transformType == 2)
        scale(x, y, z, transformInfo->transformVector);
}

void rotate(int32_t *x, int32_t *y, int32_t *z, TransformVector *vector)
{
    if (vector->x != 0)
    {
        int32_t qt_rad = fixedMul(vector->x, PI2_FIXED);
        int32_t c = fastCos(qt_rad);
        int32_t s = fastSin(qt_rad);
        int32_t temp_x = fixedMul(c, *y) - fixedMul(s, *z);
        int32_t temp_y = fixedMul(s, *y) + fixedMul(c, *z);
        *y = temp_x;
        *z = temp_y;
    }
    if (vector->y != 0)
    {
        int32_t qt_rad = fixedMul(vector->y, PI2_FIXED);
        int32_t c = fastCos(qt_rad);
        int32_t s = fastSin(qt_rad);
        int32_t temp_x = fixedMul(c, *x) - fixedMul(s, *z);
        int32_t temp_y = fixedMul(s, *x) + fixedMul(c, *z);
        *x = temp_x;
        *z = temp_y;
    }
    if (vector->z != 0)
    {
        int32_t qt_rad = fixedMul(vector->z, PI2_FIXED);
        int32_t c = fastCos(qt_rad);
        int32_t s = fastSin(qt_rad);
        int32_t temp_x = fixedMul(c, *x) - fixedMul(s, *y);
        int32_t temp_y = fixedMul(s, *x) + fixedMul(c, *y);
        *x = temp_x;
        *y = temp_y;
    }
}

void translate(int32_t *x, int32_t *y, int32_t *z, TransformVector *vector)
{
    *x += vector->x;
    *y += vector->y;
    *z += vector->z;
}

void scale(int32_t *x, int32_t *y, int32_t *z, TransformVector *vector)
{
    *x = fixedMul(*x, vector->x);
    *y = fixedMul(*y, vector->y);
    *z = fixedMul(*z, vector->z);
}

void inf(float *x, float *y, float qt)
{
    float qt_rad = qt * PI2;
    *x += 2.0f * (fastCos(qt_rad));
    *y += 2.0f * fastCos(qt_rad) * fastSin(qt_rad);
}

int checkIfTriangleVisible(Triangle2D *triangle)
{
    int e1x = triangle->b.x - triangle->a.x;
    int e1y = triangle->b.y - triangle->a.y;
    int e2x = triangle->c.x - triangle->a.x;
    int e2y = triangle->c.y - triangle->a.y;

    return (e1x * e2y - e1y * e2x) >= 0;
}

void shading(uint16_t *color, int32_t lightDistance, PointLight *light)
{
    if (lightDistance != 32)
    {
        uint8_t rMesh = (*color >> 11) & 0x1f;
        uint8_t gMesh = (*color >> 5) & 0x3f;
        uint8_t bMesh = *color & 0x1f;

        uint8_t rLight = (light->color >> 11) & 0x1f;
        uint8_t gLight = (light->color >> 5) & 0x3f;
        uint8_t bLight = light->color & 0x1f;

        uint8_t r = (rMesh * rLight) / 31;
        uint8_t g = (gMesh * gLight) / 63;
        uint8_t b = (bMesh * bLight) / 31;

        r = (uint8_t)(fixedMul(r * lightDistance, light->intensity) / SCALE_FACTOR);
        g = (uint8_t)(fixedMul(g * lightDistance, light->intensity) / SCALE_FACTOR);
        b = (uint8_t)(fixedMul(b * lightDistance, light->intensity) / SCALE_FACTOR);

        *color = (r << 11) | (g << 5) | b;
    }
}

uint16_t texturing(Triangle2D *triangle, Material *mat, int32_t divider, int x, int y)
{
    int32_t Ba = ((triangle->b.y - triangle->c.y) * (x - triangle->c.x) + (triangle->c.x - triangle->b.x) * (y - triangle->c.y)) * SCALE_FACTOR;
    int32_t Bb = ((triangle->c.y - triangle->a.y) * (x - triangle->c.x) + (triangle->a.x - triangle->c.x) * (y - triangle->c.y)) * SCALE_FACTOR;
    Ba = fixedDiv(Ba, divider);
    Bb = fixedDiv(Bb, divider);
    int32_t Bc = SCALE_FACTOR - Ba - Bb;
    int uv_x = (Ba * triangle->uvA.x + Bb * triangle->uvB.x + Bc * triangle->uvC.x) * mat->textureSize;
    int uv_y = (Ba * triangle->uvA.y + Bb * triangle->uvB.y + Bc * triangle->uvC.y) * mat->textureSize;
    uv_x = uv_x / SCALE_FACTOR;
    uv_y = uv_y / SCALE_FACTOR;
    if (uv_x < 0)
        uv_x = 0;
    if (uv_x > mat->textureSize)
        uv_x = mat->textureSize - 1;
    if (uv_y < 0)
        uv_y = 0;
    if (uv_y > mat->textureSize)
        uv_y = mat->textureSize - 1;
    return mat->texture[uv_y * mat->textureSize + uv_x];
}

void rasterize(int y, int x0, int x1, Triangle2D *triangle, Material *mat, int32_t lightDistance, int32_t divider, PointLight *light)
{
    if (y < 0 || y >= HEIGHT_DISPLAY)
        return;
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
        uint16_t color = 0;
        if (mat->textureSize == 0)
            color = mat->diffuse;
        else
            color = texturing(triangle, mat, divider, x, y);
        if (SHADING_ENABLED)
            shading(&color, lightDistance, light);
        draw_pixel(x, y, color);
    }
}

void tri(Triangle2D *triangle, Material *mat, int32_t lightDistance, PointLight *light)
{
    int x, y, uv;
    if (triangle->a.y > triangle->b.y)
    {
        y = triangle->a.y;
        triangle->a.y = triangle->b.y;
        triangle->b.y = y;
        x = triangle->a.x;
        triangle->a.x = triangle->b.x;
        triangle->b.x = x;

        uv = triangle->uvA.x;
        triangle->uvA.x = triangle->uvB.x;
        triangle->uvB.x = uv;
        uv = triangle->uvA.y;
        triangle->uvA.y = triangle->uvB.y;
        triangle->uvB.y = uv;
    }
    if (triangle->a.y > triangle->c.y)
    {
        y = triangle->a.y;
        triangle->a.y = triangle->c.y;
        triangle->c.y = y;
        x = triangle->a.x;
        triangle->a.x = triangle->c.x;
        triangle->c.x = x;

        uv = triangle->uvA.x;
        triangle->uvA.x = triangle->uvC.x;
        triangle->uvC.x = uv;
        uv = triangle->uvA.y;
        triangle->uvA.y = triangle->uvC.y;
        triangle->uvC.y = uv;
    }
    if (triangle->b.y > triangle->c.y)
    {
        y = triangle->b.y;
        triangle->b.y = triangle->c.y;
        triangle->c.y = y;
        x = triangle->b.x;
        triangle->b.x = triangle->c.x;
        triangle->c.x = x;

        uv = triangle->uvB.x;
        triangle->uvB.x = triangle->uvC.x;
        triangle->uvC.x = uv;
        uv = triangle->uvB.y;
        triangle->uvB.y = triangle->uvC.y;
        triangle->uvC.y = uv;
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

    int32_t divider = 0;

    if (mat->textureSize > 0)
    {
        divider = (triangle->b.y - triangle->c.y) * (triangle->a.x - triangle->c.x) + (triangle->c.x - triangle->b.x) * (triangle->a.y - triangle->c.y);
        divider *= SCALE_FACTOR;
    }

    if (triangle->a.y < triangle->b.y)
    {
        int q = 0;
        int xd = 1;
        if (triangle->b.x < triangle->a.x)
            xd = -1;
        while (y <= triangle->b.y)
        {
            rasterize(y, x, xx, triangle, mat, lightDistance, divider, light);
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

        while (y <= triangle->c.y && y < HEIGHT_DISPLAY)
        {
            rasterize(y, x, xx, triangle, mat, lightDistance, divider, light);
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

void draw_model(Mesh *mesh, PointLight *pLight)
{
    uint16_t verticesCounter = mesh->verticesCounter;
    float verticesModified[verticesCounter * 3];
    int verticesOnScreen[verticesCounter * 2];
    uint8_t vsc = 0;
    for (uint16_t i = 0; i < verticesCounter * 3; i += 3)
    {
        // calculates vertex coords in 3D space
        int32_t x = mesh->vertices[i];
        int32_t y = mesh->vertices[i + 1];
        int32_t z = mesh->vertices[i + 2];
        for (int j = 0; j < mesh->transformationsNum; j++)
        {
            transform(&x, &y, &z, &mesh->transformations[j]);
        }
        // calculates vertex coords on the screen
        verticesModified[i] = x;
        verticesModified[i + 1] = y;
        verticesModified[i + 2] = z;
        int32_t focalLenF = floatToFixed(FOCAL_LENGTH);
        z += (5 * SCALE_FACTOR);
        x = (x * focalLenF / z) + (SCALE_FACTOR * WIDTH_HALF);
        y = (y * focalLenF / z) + (SCALE_FACTOR * HEIGHT_HALF);
        verticesOnScreen[vsc] = x / SCALE_FACTOR;
        verticesOnScreen[vsc + 1] = y / SCALE_FACTOR;
        vsc += 2;
    }
    // flat shading

    for (uint16_t i = 0; i < mesh->facesCounter * 3; i += 3)
    {
        uint16_t a = mesh->faces[i];
        uint16_t b = mesh->faces[i + 1];
        uint16_t c = mesh->faces[i + 2];
        uint16_t uvA = mesh->uv[i];
        uint16_t uvB = mesh->uv[i + 1];
        uint16_t uvC = mesh->uv[i + 2];
        Triangle2D triangle =
            {
                {verticesOnScreen[a * 2],
                 verticesOnScreen[a * 2 + 1]},
                {verticesOnScreen[b * 2],
                 verticesOnScreen[b * 2 + 1]},
                {verticesOnScreen[c * 2],
                 verticesOnScreen[c * 2 + 1]},
                {mesh->textureCoords[uvA * 2],
                 mesh->textureCoords[uvA * 2 + 1]},
                {mesh->textureCoords[uvB * 2],
                 mesh->textureCoords[uvB * 2 + 1]},
                {mesh->textureCoords[uvC * 2],
                 mesh->textureCoords[uvC * 2 + 1]}};
        if (!checkIfTriangleVisible(&triangle))
            continue;
        // normal vector
        int32_t lightDistance = 0;
        if (SHADING_ENABLED)
        {
            int32_t xab = verticesModified[b * 3] - verticesModified[a * 3];
            int32_t yab = verticesModified[b * 3 + 1] - verticesModified[a * 3 + 1];
            int32_t zab = verticesModified[b * 3 + 2] - verticesModified[a * 3 + 2];
            int32_t xac = verticesModified[c * 3] - verticesModified[a * 3];
            int32_t yac = verticesModified[c * 3 + 1] - verticesModified[a * 3 + 1];
            int32_t zac = verticesModified[c * 3 + 2] - verticesModified[a * 3 + 2];
            int32_t normalVector[3] =
                {
                    (fixedMul(yab, zac) - fixedMul(zab, yac)),
                    (fixedMul(zab, xac) - fixedMul(xab, zac)),
                    (fixedMul(xab, yac) - fixedMul(yab, xac))};
            normalize_vector(normalVector);
            int32_t normalVectorLength = vector3_length(normalVector);
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
            int32_t center[3];
            triangle_center(&triangle3D, center);
            int32_t lightDirection[3] =
                {
                    (pLight->position.x * SCALE_FACTOR) - center[0],
                    (pLight->position.y * SCALE_FACTOR) - center[1],
                    (pLight->position.z * SCALE_FACTOR) - center[2]};
            int32_t lightLength = vector3_length(lightDirection);
            // light distance
            int32_t vector[3] =
                {
                    lightDirection[0] - normalVector[0],
                    lightDirection[1] - normalVector[1],
                    lightDirection[2] - normalVector[2]};
            int64_t lightDirectionMinusNormalVector = vector3_length(vector);
            int32_t x = fixedPow(normalVectorLength) + fixedPow(lightLength) - fixedPow(lightDirectionMinusNormalVector);
            int32_t y = fixedMul(lightLength, normalVectorLength) * 2;
            lightDistance = fixedDiv(x, y);
            if (lightDistance < 0)
                lightDistance = 0;
            if (lightDistance > SHIFT_FACTOR)
                lightDistance = SHIFT_FACTOR;
        }

        tri(&triangle, mesh->mat, lightDistance, pLight);
    }
}
