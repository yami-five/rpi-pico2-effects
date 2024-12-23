#include <stdint.h>

typedef struct
{
    uint16_t diffuse;
    uint16_t *texture;
} Material;

typedef struct
{
    uint16_t verticesCounter;
    uint16_t facesCounter;
    int *vertices;
    uint16_t *faces;
    uint16_t *textureCoords;
    uint16_t *uv;
    Material *mat;
} Mesh;

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

float fast_sqrt(float x);
float vector3_length(int *vector3);
void normalize_vector(int *vector3);
void triangle_center(int *triangle, int *center);
void rotate(float *x, float *y, float qt);
int checkIfTriangleVisible(int *triangle);
void rasterize(int y, int x0, int x1, int *triangle, Material *mat, int lightDistance);
void tri(int *triangle, Material *mat, int lightDistance);
void draw_model(Mesh *mesh, uint32_t t);