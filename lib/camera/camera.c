#include "pico/stdlib.h"
#include "fpa.h"
#include "camera.h"
// #include "models.h"
#include <stdlib.h>

#define ZNEAR 102        // floatToFixed(0.1f)
#define ZFAR 102400      // floatToFixed(100.0f)
#define ASPECTRATIO 1024 // 1:1
#define TANFOV2 1658     // tan(fov/2)

Vector3 *calculateForwardVector(Vector3 *pos, Vector3 *target)
{
    Vector3 *result = subVectors(pos, target);
    normVector(result);
    return result;
}

Vector3 *calculateRightVector(Vector3 *up, Vector3 *forward)
{
    Vector3 *result = mulVectors(up, forward);
    normVector(result);
    return result;
}

Vector3 *calculateUpVector(Vector3 *forward, Vector3 *right)
{
    Vector3 *result = mulVectors(forward, right);
    normVector(result);
    return result;
}

void calculateViewMatrix(Camera *camera)
{
    camera->vMatrix[0] = camera->right->x;
    camera->vMatrix[1] = camera->right->y;
    camera->vMatrix[2] = camera->right->z;
    camera->vMatrix[3] = -mulVectorsScalar(camera->right, camera->pos);
    camera->vMatrix[4] = camera->up->x;
    camera->vMatrix[5] = camera->up->y;
    camera->vMatrix[6] = camera->up->z;
    camera->vMatrix[7] = -mulVectorsScalar(camera->up, camera->pos);
    camera->vMatrix[8] = camera->forward->x;
    camera->vMatrix[9] = camera->forward->y;
    camera->vMatrix[10] = camera->forward->z;
    camera->vMatrix[11] = -mulVectorsScalar(camera->forward, camera->pos);
    camera->vMatrix[12] =
        camera->vMatrix[13] =
            camera->vMatrix[14] = 0;
    camera->vMatrix[15] = SCALE_FACTOR;
}

void calculatePerspectiveMatrix(Camera *camera)
{
    camera->pMatrix[0] = fixedDiv(SCALE_FACTOR, fixedMul(TANFOV2, ASPECTRATIO));
    camera->pMatrix[1] =
        camera->pMatrix[2] =
            camera->pMatrix[3] = 0;
    camera->pMatrix[4] = 0;
    camera->pMatrix[5] = fixedDiv(SCALE_FACTOR, TANFOV2);
    camera->pMatrix[6] =
        camera->pMatrix[7] = 0;
    camera->pMatrix[8] =
        camera->pMatrix[9] = 0;
    camera->pMatrix[10] = -fixedDiv((ZFAR + ZNEAR), (ZFAR - ZNEAR));
    camera->pMatrix[11] = -fixedDiv(2 * fixedMul(ZFAR, ZNEAR), (ZFAR - ZNEAR));
    camera->pMatrix[12] =
        camera->pMatrix[13] = 0;
    camera->pMatrix[14] = -SCALE_FACTOR;
    camera->pMatrix[15] = 0;
}

Camera *createCamera(int camX, int camY, int camZ, int targetX, int targetY, int targetZ, int upX, int upY, int upZ)
{
    Camera *cam = (Camera *)malloc(sizeof(Camera));
    cam->pos=(Vector3 *)malloc(sizeof(Vector3));
    cam->pos->x = camX;
    cam->pos->y = camY;
    cam->pos->z = camZ;
    cam->target=(Vector3 *)malloc(sizeof(Vector3));
    cam->target->x = targetX;
    cam->target->y = targetY;
    cam->target->z = targetZ;
    cam->up=(Vector3 *)malloc(sizeof(Vector3));
    cam->up->x = upX;
    cam->up->y = upY;
    cam->up->z = upZ;
    cam->forward = calculateForwardVector(cam->pos, cam->target);
    cam->right = calculateRightVector(cam->up, cam->forward);
    cam->up = calculateUpVector(cam->forward, cam->right);
    cam->vMatrix = (int32_t *)malloc(sizeof(int32_t) * 16);
    cam->pMatrix = (int32_t *)malloc(sizeof(int32_t) * 16);
    calculateViewMatrix(cam);
    calculatePerspectiveMatrix(cam);
    return cam;
}