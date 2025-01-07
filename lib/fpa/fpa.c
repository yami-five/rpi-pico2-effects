#include "fpa.h"

float sin_table[TABLE_SIZE];
float cos_table[TABLE_SIZE];

int32_t floatToFixed(float value)
{
    return (int32_t)(value * (1 << SHIFT_FACTOR));
}

int32_t fixedToFloat(int32_t value)
{
    return (float)value / (1 << SHIFT_FACTOR);
}

int32_t fixedAdd(int32_t a, int32_t b)
{
    return a+b;
}

int32_t fixedAdd3(int32_t a, int32_t b, int32_t c)
{
    return a+b+c;
}

int32_t fixedSub(int32_t a, int32_t b)
{
    return a-b;
}

int32_t fixedMul(int32_t a, int32_t b)
{
    int32_t fixedA = floatToFixed(a);
    int32_t fixedB = floatToFixed(b);
    int64_t result = (int64_t)fixedA * fixedB;
    return (int32_t)(result/SCALE_FACTOR);
}

int32_t fixedDiv(int32_t a, int32_t b)
{
    int32_t fixedA = floatToFixed(a);
    int32_t fixedB = floatToFixed(b);
    int64_t result = ((int64_t)fixedA * SCALE_FACTOR) / fixedB;
    return (int32_t)result;
}

int32_t fixedPow(int32_t a)
{
    return fixedMul(a, a);
}

void initSinCos()
{
    for (int32_t i = 0; i < TABLE_SIZE; i++)
    {
        float angle = i * RESOLUTION;
        sin_table[i] = sinf(angle);
        cos_table[i] = cosf(angle);
    }
}

float fastSin(float value)
{
    int32_t index = (int32_t)(value / RESOLUTION) % TABLE_SIZE;
    if (index < 0)
        index += TABLE_SIZE;
    return sin_table[index];
}

float fastCos(float value)
{
    int32_t index = (int32_t)(value / RESOLUTION) % TABLE_SIZE;
    if (index < 0)
        index += TABLE_SIZE;
    return cos_table[index];
}

float fastInvSqrt(float value)
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = value * 0.5F;
    y = value;
    i = *(long *)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float *)&i;
    y = y * (threehalfs - (x2 * y * y));
    return y;
}

float fastSqrt(float value)
{
    return value * fastInvSqrt(value);
}