#include "models.h"

typedef struct
{
    int32_t x;
    int32_t y;
    int32_t z;
} Vertex3D;

typedef struct
{
    int32_t x;
    int32_t y;
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
    uint32_t intensity;
    uint16_t color;
} PointLight;

PointLight *createLight(float x, float y, float z, uint8_t intensity, uint16_t color);
int32_t vector3_length(int32_t *vector3);
void normalize_vector(int32_t *vector3);
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