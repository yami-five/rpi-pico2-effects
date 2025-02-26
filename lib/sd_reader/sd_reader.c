#include "ff.h"
#include "fpa.h"
#include "DEV_Config.h"
#include "sd_reader.h"
#include <stdlib.h>
#include "blink.h"
#include "painter.h"

FRESULT f_res;
FATFS microSDFatFs;

uint8_t readline(uint8_t *line, uint8_t length, FIL *file)
{
	uint br;
	uint8_t lineBuffer[1];
	f_read(file, lineBuffer, 1, &br);
	if (f_eof(file))
	{
		return 1;
	}
	line[0] = lineBuffer[0];
	uint8_t counter = 1;
	while ((lineBuffer[0] != '\n') && (counter < length - 1))
	{
		f_read(file, lineBuffer, 1, &br);
		line[counter] = lineBuffer[0];
		counter++;
	}
	return 0;
}

void clearLine(uint8_t *line, uint8_t length)
{
	for (uint8_t i = 0; i < length; i++)
	{
		line[i] = 0;
	}
}

void sdInit()
{
	DEV_Digital_Write(SD_CS_PIN, 1);
	DEV_Digital_Write(LCD_CS_PIN, 1);
	DEV_Digital_Write(TP_CS_PIN, 1);

	int counter = 0;
	// Check the mounted device
	f_res = f_mount(&microSDFatFs, (TCHAR const *)"/", 1);
	if (f_res != FR_OK)
	{
		printf("SD card mount file system failed ,error code :(%d)\r\n", f_res);
	}
	else
	{
		printf("SD card mount file system success!! \r\n");
	}
}

void sdClose()
{
	f_unmount((TCHAR const *)"/");
}

LoadedObj *loadObjFile(char *file_name)
{
	FIL file;
	f_res = f_open(&file, file_name, FA_READ);
	if (f_res != FR_OK)
	{
		printf("Loading file failed :(%d)\r\n", f_res);
		return 0;
	}

	uint8_t linelength = 32;
	uint8_t line[linelength];
	// uint8_t *line = (uint8_t *)malloc(sizeof(uint8_t) * linelength);

	LoadedObj *obj = (LoadedObj *)malloc(sizeof(LoadedObj));
	obj->faces = (uint16_t *)malloc(sizeof(uint16_t) * 0);
	obj->facesCounter = 0;
	obj->vertices = (uint32_t *)malloc(sizeof(uint32_t) * 0);
	obj->verticesCounter = 0;
	obj->textureCoords = (uint32_t *)malloc(sizeof(uint32_t) * 0);
	obj->uv = (uint16_t *)malloc(sizeof(uint16_t) * 0);
	obj->textureCoordsCounter = 0;
	clearLine(line, linelength);

	while (!readline(line, linelength, &file))
	{
		if (line[0] == 'v' && line[1] == ' ')
		{
			float x, y, z;
			int result = sscanf(line + 2, "%f %f %f", &x, &y, &z);
			obj->verticesCounter++;
			// uint32_t *temp = realloc(obj->vertices, obj->verticesCounter * 3 * sizeof(uint32_t));
			// obj->vertices = temp;
			obj->vertices = realloc(obj->vertices, obj->verticesCounter * 3 * sizeof(uint32_t));
			obj->vertices[(obj->verticesCounter - 1) * 3] = floatToFixed(x);
			obj->vertices[(obj->verticesCounter - 1) * 3 + 1] = floatToFixed(y);
			obj->vertices[(obj->verticesCounter - 1) * 3 + 2] = floatToFixed(z);
		}
		else if (line[0] == 'v' && line[1] == 't')
		{
			float x, y;
			int result = sscanf(line + 3, "%f %f", &x, &y);
			obj->textureCoordsCounter++;
			obj->textureCoords = realloc(obj->textureCoords, obj->textureCoordsCounter * 2 * sizeof(uint32_t));
			// uint32_t *temp = realloc(obj->textureCoords, obj->textureCoordsCounter * 2 * sizeof(uint32_t));
			// obj->textureCoords = temp;
			obj->textureCoords[(obj->textureCoordsCounter - 1) * 2] = floatToFixed(x);
			obj->textureCoords[(obj->textureCoordsCounter - 1) * 2 + 1] = floatToFixed(y);
		}
		else if (line[0] == 'f' && line[1] == ' ')
		{
			uint16_t f[3], vt[3], vn[3];
			int result = sscanf(line + 2, "%d/%d/%d %d/%d/%d %d/%d/%d", &f[0], &vt[0], &vn[0], &f[1], &vt[1], &vn[1], &f[2], &vt[2], &vn[2]);
			obj->facesCounter++;
			obj->faces = realloc(obj->faces, obj->facesCounter * 3 * sizeof(uint16_t));
			// uint16_t *temp1 = realloc(obj->faces, obj->facesCounter * 3 * sizeof(uint16_t));
			// obj->faces = temp1;
			obj->faces[(obj->facesCounter - 1) * 3] = (f[0] - 1);
			obj->faces[(obj->facesCounter - 1) * 3 + 1] = (f[1] - 1);
			obj->faces[(obj->facesCounter - 1) * 3 + 2] = (f[2] - 1);
			obj->uv = realloc(obj->uv, obj->facesCounter * 3 * sizeof(uint16_t));
			// uint16_t *temp2 = realloc(obj->uv, obj->facesCounter * 3 * sizeof(uint16_t));
			// obj->uv = temp2;
			obj->uv[(obj->facesCounter - 1) * 3] = (vt[0] - 1);
			obj->uv[(obj->facesCounter - 1) * 3 + 1] = (vt[1] - 1);
			obj->uv[(obj->facesCounter - 1) * 3 + 2] = (vt[2] - 1);
		}
		clearLine(line, linelength);
	}
	f_close(&file);
	// free(line);
	return obj;
}

LoadedBmp *loadBmpFile(char *file_name)
{
	FIL file;
	f_res = f_open(&file, file_name, FA_READ);
	if (f_res != FR_OK)
	{
		printf("Loading file failed :(%d)\r\n", f_res);
		return 0;
	}
	LoadedBmp *bmp = (LoadedBmp *)malloc(sizeof(LoadedBmp));
	int br;
	uint32_t pixelArrayOffset[1];
	f_lseek(&file, 10);
	f_read(&file, pixelArrayOffset, 4, &br);
	uint32_t buffer[1];
	// f_read(&file, buffer, 4, &br);
	f_lseek(&file, 18);
	f_read(&file, buffer, 4, &br);
	bmp->sizeY = buffer[0];
	f_read(&file, buffer, 4, &br);
	bmp->sizeX = buffer[0];
	f_lseek(&file, pixelArrayOffset[0]);
	uint32_t pixelsNum = bmp->sizeX * bmp->sizeY;
	bmp->pixels = (uint16_t *)malloc(sizeof(uint16_t) * pixelsNum);
	f_read(&file, bmp->pixels, pixelsNum, &br);
	f_close(&file);
	return bmp;
}

void drawBmpFilePerLine(char *file_name)
{
	FIL file;
	f_res = f_open(&file, file_name, FA_READ);
	int br;
	uint32_t pixelArrayOffset[1];
	f_lseek(&file, 10);
	f_read(&file, pixelArrayOffset, 4, &br);
	uint32_t buffer[1];
	// f_read(&file, buffer, 4, &br);
	f_lseek(&file, 18);
	f_read(&file, buffer, 4, &br);
	uint16_t width = buffer[0];
	f_read(&file, buffer, 4, &br);
	uint16_t height = buffer[0];
	f_lseek(&file, pixelArrayOffset[0]);
	uint16_t buf[1];
	for (uint16_t y = 0; y < height; y++)
	{
		for (uint16_t x = 0; x < width; x++)
		{
			f_read(&file, buf, 2, &br);
			uint16_t color = buf[0];
			uint16_t fixedColor = ((color & 0xF800) >> 11) |
                      ((color & 0x07E0))      |
                      ((color & 0x001F) << 11); 
			// uint8_t b = color & 0x1f;
			// uint8_t g = (color >> 5) & 0x3f;
			// uint8_t r = (color >> 11) & 0x1f;
			// color = (r << 11) | (g << 5) | b;
			draw_pixel(x, y, color);
		}
	}
	f_close(&file);
}