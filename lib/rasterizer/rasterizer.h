#include "models.h"
#include "vectors.h"

typedef struct
{
    Vector2 a;
    Vector2 b;
    Vector2 c;
    Vector2 uvA;
    Vector2 uvB;
    Vector2 uvC;
} Triangle2D;

typedef struct
{
    Vector3 a;
    Vector3 b;
    Vector3 c;
} Triangle3D;

typedef struct
{
    Vector3 position;
    uint32_t intensity;
    uint16_t color;
} PointLight;

PointLight *createLight(int x, int y, int z, uint8_t intensity, uint16_t color);
// int32_t vector3_length(int32_t *vector3);
// void normalize_vector(int32_t *vector3);
void triangle_center(Triangle3D *triangle, int32_t *center);
void transform(int32_t *x, int32_t *y, int32_t *z, TransformInfo *transformInfo);
void rotate(int32_t *x, int32_t *y, int32_t *z, TransformVector * TransformVector);
void translate(int32_t *x, int32_t *y, int32_t *z, TransformVector * TransformVector);
void scale(int32_t *x, int32_t *y, int32_t *z, TransformVector * TransformVector);
void inf(float *x, float *y, float qt);
int checkIfTriangleVisible(Triangle2D *triangle);
void rasterize(int y, int x0, int x1, Triangle2D *triangle, Material *mat, int32_t lightDistance, int32_t divider, PointLight *light);
void tri(Triangle2D *triangle, Material *mat, int32_t lightDistance, PointLight *light);
void draw_model(Mesh *mesh, PointLight *pLight);
void shading(uint16_t *color, int32_t lightDistance, PointLight *light);
uint16_t texturing(Triangle2D *triangle, Material *mat, int32_t divider, int x, int y);