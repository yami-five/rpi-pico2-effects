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

typedef struct
{
    uint16_t sizeX;
    uint16_t sizeY;
    uint16_t *pixels;
} LoadedBmp;

uint8_t readline(uint8_t *line, uint8_t length, FIL *file);
void clearLine(uint8_t *line, uint8_t length);
void sdInit();
void sdClose();
LoadedObj *loadObjFile(char *file_name);
LoadedBmp *loadBmpFile(char *file_name);
void drawBmpFilePerLine(char *file_name);