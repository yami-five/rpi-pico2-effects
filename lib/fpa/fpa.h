#include <math.h>
#include <stdint.h>

#define SHIFT_FACTOR 6
#define SCALE_FACTOR (1<<SHIFT_FACTOR)
#define TABLE_SIZE 36000
#define PI 3.14159265358979323846f
#define PI2 (PI * 2.0f)
#define RESOLUTION (PI2 / TABLE_SIZE)

int32_t floatToFixed(float value);
int32_t fixedToFloat(int32_t value);
int32_t fixedAdd(int32_t a, int32_t b);
int32_t fixedAdd3(int32_t a, int32_t b, int32_t c);
int32_t fixedSub(int32_t a, int32_t b);
int32_t fixedMul(int32_t a, int32_t b);
int32_t fixedDiv(int32_t a, int32_t b);
int32_t fixedPow(int32_t a);
void initSinCos();
float fastSin(float value);
float fastCos(float value);
float fastInvSqrt(float number);
float fastSqrt(float number);