#include <math.h>
#include <stdint.h>

#define SHIFT_FACTOR 10
#define SCALE_FACTOR (1 << SHIFT_FACTOR)
#define TABLE_SIZE 36000
#define PI 3.14159265358979323846f
#define PI2 (PI * 2.0f)
#define PI_FIXED PI *SCALE_FACTOR
#define PI2_FIXED PI2 *SCALE_FACTOR
#define RESOLUTION (PI2 / TABLE_SIZE)
#define RESOLUTION_FIXED RESOLUTION *SCALE_FACTOR

int32_t floatToFixed(float value);
int32_t fixedToFloat(int32_t value);
int32_t fixedAdd(int32_t a, int32_t b);
int32_t fixedAdd3(int32_t a, int32_t b, int32_t c);
int32_t fixedSub(int32_t a, int32_t b);
int64_t fixedMul(int32_t a, int32_t b);
int32_t fixedDiv(int32_t a, int32_t b);
int64_t fixedPow(int32_t a);
void initSinCos();
int32_t fastSin(int32_t value);
int32_t fastCos(int32_t value);
int32_t fastInvSqrt(int32_t number);
int32_t fastSqrt(int64_t number);