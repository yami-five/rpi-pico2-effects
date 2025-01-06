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
    float textureCoords[8] = {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f};
    uint16_t uv[36] = {2, 1, 0, 2, 1, 0, 1, 3, 2, 0, 2, 3, 2, 1, 0, 1, 3, 2, 1, 3, 0, 1, 3, 0, 3, 0, 2, 2, 1, 3, 1, 3, 0, 3, 0, 2};

    cube->verticesCounter = 8;
    cube->facesCounter = 12;
    cube->vertices = (float *)malloc(sizeof(float) * 24);
    cube->faces = (uint16_t *)malloc(sizeof(uint16_t) * 36);
    cube->textureCoords = (float *)malloc(sizeof(float) * 8);
    cube->uv = (uint16_t *)malloc(sizeof(uint16_t) * 36);
    cube->mat = mat;
    cube->transformations = NULL;
    cube->transformationsNum = 0;

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

Mesh *createTexturedPlane(uint16_t *texture, int textureSize)
{
    Material *material = (Material *)malloc(sizeof(Material));
    material->diffuse = 0;
    material->texture = texture;
    material->textureSize = textureSize;
    return createPlane(material);
}

Mesh *createPlane(Material *mat)
{
    Material material = *mat;
    Mesh *plane = (Mesh *)malloc(sizeof(Mesh));
    float vertices[12] = {-1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, -1.0f, 1.0f, 0.0f, -1.0f};
    uint16_t faces[6] = {0, 2, 1, 2, 3, 1};
    float textureCoords[8] = {0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f};
    uint16_t uv[6] = {2, 1, 0, 1, 3, 0};

    plane->verticesCounter = 4;
    plane->facesCounter = 2;
    plane->vertices = (float *)malloc(sizeof(float) * 12);
    plane->faces = (uint16_t *)malloc(sizeof(uint16_t) * 6);
    plane->textureCoords = (float *)malloc(sizeof(float) * 8);
    plane->uv = (uint16_t *)malloc(sizeof(uint16_t) * 6);
    plane->mat = mat;
    plane->transformations = NULL;
    plane->transformationsNum = 0;

    for (int i = 0; i < 8; i++)
    {
        plane->textureCoords[i] = textureCoords[i];
    }
    for (int i = 0; i < 6; i++)
    {
        plane->faces[i] = faces[i];
        plane->uv[i] = uv[i];
        plane->vertices[i * 2] = vertices[i * 2];
        plane->vertices[i * 2 + 1] = vertices[i * 2 + 1];
    }
    return plane;
}

TransformInfo *addTransformation(TransformInfo *currentTransformations, int *currentTransformationsNum, float x, float y, float z, uint8_t transformationType)
{
    if (transformationType > 2)
        return currentTransformations;

    *currentTransformationsNum += 1;
    TransformInfo *newTransformations = (TransformInfo *)realloc(currentTransformations, *currentTransformationsNum * sizeof(TransformInfo));
    newTransformations[*currentTransformationsNum - 1].transformVector = (TransformVector *)malloc(sizeof(TransformVector));
    newTransformations[*currentTransformationsNum - 1].transformType = transformationType;
    newTransformations[*currentTransformationsNum - 1].transformVector->x = x;
    newTransformations[*currentTransformationsNum - 1].transformVector->y = y;
    newTransformations[*currentTransformationsNum - 1].transformVector->z = z;

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