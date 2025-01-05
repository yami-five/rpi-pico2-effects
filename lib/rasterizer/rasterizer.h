#include "models.h"

typedef struct
{
    uint16_t x;
    uint16_t y;
    uint16_t z;
} Vertex3D;

typedef struct
{
    uint16_t x;
    uint16_t y;
} Vertex2D;

typedef struct
{
    Vertex2D a;
    Vertex2D b;
    Vertex2D c;
    Vertex2D uvA;
    Vertex2D uvB;
    Vertex2D uvC;
} Triangle2D;

typedef struct
{
    Vertex3D a;
    Vertex3D b;
    Vertex3D c;
} Triangle3D;

typedef struct
{
    Vertex3D position;
    float intensity;
    uint16_t color;
} PointLight;

PointLight *createLight(int x, int y, int z, uint8_t intensity, uint16_t color);
float fast_sqrt(float x);
float vector3_length(float *vector3);
void normalize_vector(float *vector3);
void triangle_center(Triangle3D *triangle, float *center);
void transform(float *x, float *y, float *z, TransformInfo *transformInfo);
void rotate(float *x, float *y, float *z, TransformVector * TransformVector);
void translate(float *x, float *y, float *z, TransformVector * TransformVector);
void scale(float *x, float *y, float *z, TransformVector * TransformVector);
void inf(float *x, float *y, float qt);
int checkIfTriangleVisible(Triangle2D *triangle);
void rasterize(int y, int x0, int x1, Triangle2D *triangle, Material *mat, float lightDistance, float divider, PointLight *light);
void tri(Triangle2D *triangle, Material *mat, float lightDistance, PointLight *light);
void draw_model(Mesh *mesh, PointLight *pLight);
void shading(uint16_t *color, float lightDistance, PointLight *light);
uint16_t texturing(Triangle2D *triangle, Material *mat, float divider, int x, int y);