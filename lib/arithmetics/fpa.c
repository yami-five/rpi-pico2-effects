#include "fpa.h"

int32_t sin_table[TABLE_SIZE];
int32_t cos_table[TABLE_SIZE];

int32_t floatToFixed(float value)
{
    return (int32_t)(value * SCALE_FACTOR);
}

int32_t fixedToFloat(int32_t value)
{
    return (float)value / SCALE_FACTOR;
}

int32_t fixedAdd(int32_t a, int32_t b)
{
    return a + b;
}

int32_t fixedAdd3(int32_t a, int32_t b, int32_t c)
{
    return a + b + c;
}

int32_t fixedSub(int32_t a, int32_t b)
{
    return a - b;
}

int64_t fixedMul(int32_t a, int32_t b)
{
    int64_t result = (int64_t)a * b;
    return (int64_t)(result / SCALE_FACTOR);
}

int32_t fixedDiv(int32_t a, int32_t b)
{
    int64_t result = ((int64_t)a * SCALE_FACTOR) / b;
    return (int32_t)result;
}

int64_t fixedPow(int32_t a)
{
    return fixedMul(a, a);
}

void initSinCos()
{
    for (int32_t i = 0; i < TABLE_SIZE; i++)
    {
        float angle = i * RESOLUTION;
        sin_table[i] = floatToFixed(sinf(angle));
        cos_table[i] = floatToFixed(cosf(angle));
    }
}

int32_t fastSin(int32_t value)
{
    int32_t index = value % TABLE_SIZE;
    if (index < 0)
        index += TABLE_SIZE;
    return sin_table[index];
}

int32_t fastCos(int32_t value)
{
    int32_t index = value % TABLE_SIZE;
    if (index < 0)
        index += TABLE_SIZE;
    return cos_table[index];
}

int32_t fastInvSqrt(int32_t value)
{
    int32_t x2, y;
    const int32_t threehalfs = 96;
    x2 = value >> 1;

    y = 12288 - (value >> 1);

    int32_t ySquared = fixedPow(y);
    int32_t product = fixedMul(x2, ySquared);
    int32_t correction = threehalfs - product;
    y = fixedMul(y, correction);

    return y;
}

int32_t fastSqrt(int64_t value)
{
    if (value <= 0)
        return 0;
    int64_t x = value;
    for (uint8_t i = 0; i < 5; i++)
    {
        x = (x + fixedDiv(value, x)) >> 1;
    }
    return x;
}