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
    uint16_t verticesCounter;
    uint16_t facesCounter;
    float *vertices;
    uint16_t *faces;
    uint16_t *textureCoords;
    uint16_t *uv;
    Material *mat;
} Mesh;

Mesh* createColoredCube(uint16_t color);
Mesh* createTexturedCube(uint16_t *texture, int textureSize);
Mesh* createCube(Material *mat);

void freeModel(Mesh* mesh);
#endif