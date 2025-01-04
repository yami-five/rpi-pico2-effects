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
    uint8_t intensity;
    uint16_t color;
} PointLight;

PointLight createLight(int x, int y, int z, uint8_t intensity, uint16_t color);
float fast_sqrt(float x);
float vector3_length(float *vector3);
void normalize_vector(float *vector3);
void triangle_center(Triangle3D *triangle, float *center);
void rotate(float *x, float *y, float qt);
void inf(float *x, float *y, float qt);
int checkIfTriangleVisible(Triangle2D *triangle);
void rasterize(int y, int x0, int x1, Triangle2D *triangle, Material *mat, float lightDistance);
void tri(Triangle2D *triangle, Material *mat, float lightDistance);
void draw_model(Mesh *mesh, PointLight *pLight, uint32_t t);
void lightedColor(uint16_t *color, float lightDistance);