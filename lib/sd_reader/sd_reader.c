#include "ff.h"
#include "DEV_Config.h"

FRESULT f_res;
FATFS microSDFatFs;

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
}