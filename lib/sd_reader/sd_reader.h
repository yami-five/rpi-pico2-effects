#include "ff.h"

typedef struct
{
    uint16_t verticesCounter;
    uint16_t facesCounter;
    uint16_t textureCoordsCounter;
    int32_t *vertices;
    uint16_t *faces;
    int32_t *textureCoords;
    uint16_t *uv;
} LoadedObj;

uint8_t readline(uint8_t * line,uint8_t length,FIL * file);
void clearLine(uint8_t * line, uint8_t length);
void sdInit();
LoadedObj * loadObjFile(char * file_name);