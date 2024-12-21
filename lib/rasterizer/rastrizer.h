typedef struct
{
    uint16_t diffuse;
    uint16_t *texture;
} Material;

typedef struct
{
    uint16_t verticesCounter;
    uint16_t facesCounter;
    uint16_t *vertices;
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
int vector3_length(int vector3[3]);
void normalize_vector(int *vector3[3]);
int *triangle_center(int triangle[9]);
void rotate(int16_t *x, uint16_t *y, uint32_t t);
bool checkIfTriangleVisible(int triangle[6]);
void tri(int triangle[6], Material *mat, int lightDistance);
void draw_model(Mesh *mesh, uint32_t t);