#include "models.h"
#include "vectors.h"

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
Vector3 *calculateForwardVector(Vector3 *pos, Vector3 *target);
Vector3 *calculateRightVector(Vector3 *up, Vector3 *forward);
Vector3 *calculateUpVector(Vector3 *forward, Vector3 *right);
void calculateViewMatrix(Camera *camera);
void calculatePerspectiveMatrix(Camera *camera);

void initCamera(Camera *camera);