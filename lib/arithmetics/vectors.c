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

void fixedMulMatrixVector(int32_t *x, int32_t *y, int32_t *z, int32_t *w, int32_t *matrix)
{
    int32_t resultX = 0;
    int32_t resultY = 0;
    int32_t resultZ = 0;
    int32_t resultW = 0;

    resultX+=fixedMul(*x,matrix[0])+fixedMul(*y,matrix[1])+fixedMul(*z,matrix[2])+fixedMul(*w,matrix[3]);
    resultY+=fixedMul(*x,matrix[4])+fixedMul(*y,matrix[5])+fixedMul(*z,matrix[6])+fixedMul(*w,matrix[7]);
    resultZ+=fixedMul(*x,matrix[8])+fixedMul(*y,matrix[9])+fixedMul(*z,matrix[10])+fixedMul(*w,matrix[11]);
    resultW+=fixedMul(*x,matrix[12])+fixedMul(*y,matrix[13])+fixedMul(*z,matrix[14])+fixedMul(*w,matrix[15]);
    
    *x = resultX;
    *y = resultY;
    *z = resultZ;
    *w = resultW;
}