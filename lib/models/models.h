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
    float x;
    float y;
    float z;
} TransformMatrix;

typedef struct
{
    uint8_t transformType;
    TransformMatrix* transformMatrix;
} TransformInfo;

typedef struct
{
    uint16_t verticesCounter;
    uint16_t facesCounter;
    float *vertices;
    uint16_t *faces;
    uint16_t *textureCoords;
    uint16_t *uv;
    Material *mat;
    TransformInfo *transformations;
    int transformationsNum;
} Mesh;

Mesh* createColoredCube(uint16_t color);
Mesh* createTexturedCube(uint16_t *texture, int textureSize);
Mesh* createCube(Material *mat);

TransformInfo *addTransformation(TransformInfo *currentTransformations, int *currentTransformationsNum, float x, float y, float z, uint8_t transformationType);

void freeModel(Mesh* mesh);
#endif