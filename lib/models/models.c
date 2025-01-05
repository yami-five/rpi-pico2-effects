#include "models.h"
#include <stdlib.h>

#define ARRAY_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))

Mesh *createColoredCube(uint16_t color)
{
    Material *material = (Material *)malloc(sizeof(Material));
    material->diffuse = color;
    material->texture = 0;
    material->textureSize = 0;
    return createCube(material);
}
Mesh *createTexturedCube(uint16_t *texture, int textureSize)
{
    Material *material = (Material *)malloc(sizeof(Material));
    material->diffuse = 0;
    material->texture = texture;
    material->textureSize = textureSize;
    return createCube(material);
}

Mesh *createCube(Material *mat)
{
    Material material = *mat;
    Mesh *cube = (Mesh *)malloc(sizeof(Mesh));
    float vertices[24] = {1.3f, 1.3f, -1.3f, 1.3f, -1.3f, -1.3f, 1.3f, 1.3f, 1.3f, 1.3f, -1.3f, 1.3f, -1.3f, 1.3f, -1.3f, -1.3f, -1.3f, -1.3f, -1.3f, 1.3f, 1.3f, -1.3f, -1.3f, 1.3f};
    uint16_t faces[36] = {0, 2, 4, 3, 7, 2, 7, 5, 6, 5, 7, 1, 1, 3, 0, 5, 1, 4, 2, 6, 4, 7, 6, 2, 5, 4, 6, 7, 3, 1, 3, 2, 0, 1, 0, 4};
    uint16_t textureCoords[8] = {1, 0, 0, 1, 0, 0, 1, 1};
    uint16_t uv[36] = {2, 1, 0, 2, 1, 0, 1, 3, 2, 0, 2, 3, 2, 1, 0, 1, 3, 2, 1, 3, 0, 1, 3, 0, 3, 0, 2, 2, 1, 3, 1, 3, 0, 3, 0, 2};

    cube->verticesCounter = 8;
    cube->facesCounter = 12;
    cube->vertices = (float *)malloc(sizeof(float) * 24);
    cube->faces = (uint16_t *)malloc(sizeof(uint16_t) * 36);
    cube->textureCoords = (uint16_t *)malloc(sizeof(uint16_t) * 8);
    cube->uv = (uint16_t *)malloc(sizeof(uint16_t) * 36);
    cube->mat = mat;
    cube->transformations = NULL;
    cube->transformationsNum=0;

    for (int i = 0; i < 8; i++)
    {
        cube->textureCoords[i] = textureCoords[i];
        cube->vertices[i * 3] = vertices[i * 3];
        cube->vertices[i * 3 + 1] = vertices[i * 3 + 1];
        cube->vertices[i * 3 + 2] = vertices[i * 3 + 2];
    }
    for (int i = 0; i < 36; i++)
    {
        cube->faces[i] = faces[i];
        cube->uv[i] = uv[i];
    }
    return cube;
}

TransformInfo *addTransformation(TransformInfo *currentTransformations, int *currentTransformationsNum, float x, float y, float z, uint8_t transformationType)
{
    if (transformationType>2)
        return currentTransformations;

    *currentTransformationsNum+=1;
    TransformInfo *newTransformations = (TransformInfo *)realloc(currentTransformations, *currentTransformationsNum * sizeof(TransformInfo));
    newTransformations->transformMatrix = (TransformMatrix *)malloc(sizeof(TransformMatrix));
    newTransformations[*currentTransformationsNum - 1].transformType = transformationType;
    newTransformations[*currentTransformationsNum - 1].transformMatrix->x = x;
    newTransformations[*currentTransformationsNum - 1].transformMatrix->y = y;
    newTransformations[*currentTransformationsNum - 1].transformMatrix->z = z;

    return newTransformations;
}

void freeModel(Mesh *mesh)
{
    free(mesh->mat);
    free(mesh->faces);
    free(mesh->vertices);
    free(mesh->textureCoords);
    free(mesh->uv);
    free(mesh->transformations);
    free(mesh);
}