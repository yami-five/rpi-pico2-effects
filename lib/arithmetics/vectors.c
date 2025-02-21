#include "vectors.h"
#include "fpa.h"
#include <stdlib.h>

Vector3 *subVectors(Vector3 *vecA, Vector3 *vecB)
{
    Vector3 *result = (Vector3 *)malloc(sizeof(Vector3));
    result->x = vecA->x - vecB->x;
    result->y = vecA->y - vecB->y;
    result->z = vecA->z - vecB->z;
    return result;
}

Vector3 *mulVectors(Vector3 *vecA, Vector3 *vecB)
{
    Vector3 *result = (Vector3 *)malloc(sizeof(Vector3));
    result->x = fixedMul(vecA->y, vecB->z) + fixedMul(vecA->z, vecB->y);
    result->y = fixedMul(vecA->z, vecB->x) + fixedMul(vecA->x, vecB->z);
    result->z = fixedMul(vecA->x, vecB->y) + fixedMul(vecA->y, vecB->x);
    return result;
}

int32_t mulVectorsScalar(Vector3 *vecA, Vector3 *vecB)
{
    return fixedMul(vecA->x,vecB->x)+fixedMul(vecA->y,vecB->y)+fixedMul(vecA->z,vecB->z);
}

int32_t lenVector(Vector3 *vec)
{
    return fastSqrt(fixedPow(vec->x) + fixedPow(vec->y) + fixedPow(vec->z));
}

void normVector(Vector3 *vec)
{
    int32_t len = lenVector(vec);
    vec->x = fixedDiv(vec->x, len);
    vec->y = fixedDiv(vec->y, len);
    vec->z = fixedDiv(vec->z, len);
}

void fixedMulMatrixVector(int32_t x, int32_t y, int32_t z, int32_t w, int32_t *matrix)
{
    int32_t resultX = 0;
    int32_t resultY = 0;
    int32_t resultZ = 0;
    int32_t resultW = 0;
    for (uint8_t i = 0; i < 4; i++)
    {
        resultX += fixedMul(matrix[i], x);
        resultY += fixedMul(matrix[i + 4], y);
        resultZ += fixedMul(matrix[i + 8], z);
        resultW += fixedMul(matrix[i + 12], w);
    }
    x = resultX;
    y = resultY;
    z = resultZ;
    w = resultW;
}