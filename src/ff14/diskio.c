/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

// #define VERILATOR 1

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */

#define VER(i) (*((volatile int *) (0x13fc000 + (i) * 4)))
#define VER0 VER(0)
#define VER1 VER(1)

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */


// SD MEM
#define SD_CMD (*((volatile int *) 0x1000020))
#define SD_DATA (*((volatile int *) 0x1000024))
#define SD_FIFO_A (*((volatile int *) 0x1000028))
#define SD_FIFO_B (*((volatile int *) 0x100002c))

// SD Defines
#define SDSETAUX 0x0ff
#define SDREADAUX 0x0bf

#define SDCMD 0x040

#define SDFIFOOP 0x800
#define SDWRITEOP 0xc00

#define SDREADREG 0x0200

#define SDALTFIFO 0x1000

#define SDBUSY 0x04000
#define SDERROR 0x08000
#define SDCLEARERR 0x08000
#define SDCRCERR 0x10000
#define SDERRTOK 0x20000

#define SDREMOVED 0x40000
#define SDPRESENTN 0x80000

#define SDGOIDLE ((SDREMOVED|SDCLEARERR|SDCMD)+0)

#define SDREADSECTOR ((SDCMD|SDCLEARERR|SDFIFOOP)+17)

#define SDWRITESECTOR ((SDCMD|SDCLEARERR|SDWRITEOP)+24)

#define SDWAITWHILEBUSY while(SD_CMD & SDBUSY)

#define SEG (*((volatile int *) 0x1000000))

void sad_wait(int ms);


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{

  return 0;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	return 0; // disk OK
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	int result;

#ifdef VERILATOR

	VER(10) = sector;

  unsigned int *my_buf = (unsigned int *) (void *) buff;

  for (int sector_i = 0; sector_i < count; sector_i++) {
    for (int i = 0; i < 128; i++) {
      my_buf[i] = VER(10);
    }
  }


#else

#define SWITCHES (*((volatile int *) 0x1000004))
  SWITCHES = sector;
  SEG = sector;
  sad_wait(1000);

  unsigned int *my_buf = (unsigned int *) (void *) buff;

  for (int sector_i = 0; sector_i < count; sector_i++) {
    SD_DATA = 0x0700;
    SD_CMD = SDSETAUX;

    SD_DATA = sector + sector_i;
    SD_CMD  = SDREADSECTOR;
    SDWAITWHILEBUSY;

    if (SD_CMD&SDERROR) {
      SEG = 0x7eade77;
      sad_wait(1000);
      return RES_ERROR;
    }

    for (int i = 0; i < 128; i++) {
      my_buf[i] = SD_FIFO_A;
    }

    my_buf += 128; // 512 bytes
  }

#endif

	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

static unsigned int swap_bytes(unsigned int a) {
  unsigned int b = 0;

  b |= ((a >> 24) & 0xFF) << 0;
  b |= ((a >> 16) & 0xFF) << 8;
  b |= ((a >> 8) & 0xFF) << 16;
  b |= ((a >> 0) & 0xFF) << 24;

  return b;
}

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;

  unsigned int *my_buf = (unsigned int *) (void *) buff;

  for (int sector_i = 0; sector_i < count; sector_i++) {
    SD_DATA = 0x0700;
    SD_CMD = SDSETAUX;

    for (int i = 0; i < 128; i++) {
      SD_FIFO_A = swap_bytes(my_buf[i]);
    }

    SD_DATA = sector + sector_i;
    SD_CMD  = SDWRITESECTOR;
    SDWAITWHILEBUSY;

    if (SD_CMD&SDERROR) {
      return RES_ERROR;
    }

    my_buf += 128; // 512 bytes
  }


  return RES_OK;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	return RES_OK; // should never be called
}


int perf_diskio(int a, int b) {
  return a + (a ^ b);
}
