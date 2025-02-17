#include "ff.h"
#include "DEV_Config.h"

FRESULT f_res;
FATFS microSDFatFs;

uint8_t readline(uint8_t *line, uint8_t length, FIL *file)
{
	uint br;
	uint8_t lineBuffer[1];
	f_read(file, lineBuffer, 1, &br);
	if (lineBuffer[0] == EOF)
		return 1;
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

void clear_line(uint8_t *line, uint8_t length)
{
	for (uint8_t i = 0; i < length; i++)
	{
		line[i] = 0;
	}
}

void sd_init()
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
	FIL file;
	f_open(&file, "cube.obj", FA_READ);

	uint8_t linelength=32;
	uint8_t line[linelength];
	clear_line(&line, linelength);

	while (!readline(&line, linelength, &file))
	{
		readline(&line, linelength, &file);
		uint8_t test = 1;
	}
}