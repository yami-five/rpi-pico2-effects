#include "vectors.h"

#ifndef CAMERA_H
#define CAMERA_H

typedef struct
{
    Vector3 *pos;
    Vector3 *target;
    Vector3 *up;
    Vector3 *right;
    Vector3 *forward;
    int32_t *vMatrix;
    int32_t *pMatrix;
} Camera;

Camera *createCamera(int camX, int camY, int camZ, int targetX, int targetY, int targetZ, int upX, int upY, int upZ);
Vector3 *calculateForwardVector(Vector3 *pos, Vector3 *target);
Vector3 *calculateRightVector(Vector3 *up, Vector3 *forward);
Vector3 *calculateUpVector(Vector3 *forward, Vector3 *right);
void calculateViewMatrix(Camera *camera);
void calculatePerspectiveMatrix(Camera *camera);

#endif