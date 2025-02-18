#include <stdint.h>
#ifndef MODELS_H
#define MODELS_H

typedef struct
{
    uint16_t diffuse;
    uint16_t *texture;
    int textureSize;
} Material;

typedef struct
{
    int32_t x;
    int32_t y;
    int32_t z;
} TransformVector;

typedef struct
{
    uint8_t transformType;
    TransformVector* transformVector;
} TransformInfo;

typedef struct
{
    uint16_t verticesCounter;
    uint16_t facesCounter;
    int32_t *vertices;
    uint16_t *faces;
    int32_t *textureCoords;
    uint16_t *uv;
    Material *mat;
    TransformInfo *transformations;
    int transformationsNum;
} Mesh;

Mesh* createColoredMesh(uint16_t color, char *filename);
Mesh* createTexturedMesh(uint16_t *texture, int textureSize, char *filename);
Mesh* createMesh(Material *mat, char *filename);

Mesh* createColoredCube(uint16_t color);
Mesh* createTexturedCube(uint16_t *texture, int textureSize);
Mesh* createCube(Material *mat);

Mesh* createTexturedPlane(uint16_t *texture, int textureSize);
Mesh* createPlane(Material *mat);

TransformInfo *addTransformation(TransformInfo *currentTransformations, int *currentTransformationsNum, float x, float y, float z, uint8_t transformationType);

void freeModel(Mesh* mesh);
#endif