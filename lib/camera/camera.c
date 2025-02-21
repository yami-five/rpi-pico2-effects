#include "pico/stdlib.h"
#include "fpa.h"
#include "camera.h"
#include "models.h"
#include <stdlib.h>

#define ZNEAR 102        // floatToFixed(0.1f)
#define ZFAR 102400      // floatToFixed(100.0f)
#define ASPECTRATIO 1365 // 4:3
#define TANFOV2 1658     // tan(fov/2)

Vector3 *calculateForwardVector(Vector3 *pos, Vector3 *target)
{
    Vector3 *result = subVectors(pos, target);
    normVector(&result);
    return result;
}
Vector3 *calculateRightVector(Vector3 *up, Vector3 *forward)
{
    Vector3 *result = mulVectors(up, forward);
    normVector(&result);
    return result;
}
Vector3 *calculateUpVector(Vector3 *forward, Vector3 *right)
{
    Vector3 *result = mulVectors(forward, right);
    normVector(&result);
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
    camera->vMatrix[8] = -camera->forward->x;
    camera->vMatrix[9] = -camera->forward->y;
    camera->vMatrix[10] = -camera->forward->z;
    camera->vMatrix[11] = mulVectorsScalar(camera->right, camera->pos);
    camera->vMatrix[12] =
        camera->vMatrix[13] =
            camera->vMatrix[14] = 0;
    camera->vMatrix[15] = 1;
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
}

void initCamera(Camera *camera);