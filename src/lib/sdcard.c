//
// Created by Will Gulian on 11/3/19.
//

#include "hardware.h"
#include "hardware_sdcard.h"
#include "util.h"

// always false, help CLion out a little bit
#ifndef SDWAITWHILEBUSY
#include "../include/hardware.h"
#include "../include/hardware_sdcard.h"
#include "../include/util.h"
#endif

static get_ocr() {
  int OCR;

  SD_DATA = 0;
  SD_CMD = (SDREADREG|SDCMD) + 58;
  SDWAITWHILEBUSY;
  OCR = SD_DATA;

  return OCR;
}

int is_card_ready() {
  return get_ocr() != 0;
}

#define VSEG(x)
// #define VSEG(x) SEG = x;


static int sdcard_do_init() {

  // SD - Clear errors

  SDWAITWHILEBUSY;
  VSEG(0xbeef1);
  SD_CMD = SDCLEARERR;
  VSEG(0xbeef2);
  SDWAITWHILEBUSY;

  // SD - Set Communication Baud Rate
  SD_DATA = 0x07c >> 1;
  VSEG(0xbeef3);
  SD_CMD = SDSETAUX;
  VSEG(0xbeef4);
  // wait not needed

//  if (is_card_ready()) {
//    return 1;
//  }

  // SD - First command, GO IDLE
  SD_DATA = 0;
  SD_CMD = SDGOIDLE;
  SDWAITWHILEBUSY;


  // SD -
  VSEG(0xbeef7);
  SD_DATA = 0x40000000;
  VSEG(0xbeef8);
  SD_CMD = SDCMD+1;
  VSEG(0xbeef9);
  SDWAITWHILEBUSY;

  // Set Voltage
  SD_DATA = 0x1af;
  SD_CMD = SDCMD+8;
  SDWAITWHILEBUSY;

  VSEG(0xbeefa);

  int max_count = 0;
  int dev_busy = 0;
  do {

    SD_DATA = 0;
    SD_CMD = SDCMD+55;
    VSEG(0xbeefb);
    SDWAITWHILEBUSY;

    SD_DATA = 0x40000000;
    SD_CMD = SDCMD+41;
    SD_DATA = 0x1a5;
    SD_CMD = SDCMD+8;
    VSEG(0xbeefc);
    SDWAITWHILEBUSY;

    dev_busy = SD_CMD & 1;
    max_count++;
  } while (dev_busy && max_count < 12);

  if (dev_busy) {
    return 0;
  }

  // SD - Read OCR

  if (!get_ocr()) {
    return 0;
  }

//  SEG = OCR;
//  sad_wait(5000);
  //SEG = 0xb0ba;

   SD_DATA = 0x0201; // 16 byte fifo, clk/4 = 12.5MHz
   SD_CMD = SDSETAUX;

  // SD - Read CSD

//  int CSD[4];
//  SD_DATA = 0;
//  SD_CMD = (SDFIFOOP|SDCMD)+9;
//  SDWAITWHILEBUSY;
//  for (int i = 0; i < 4; i++)
//    CSD[i] = SD_FIFO_A;
//
//
//  SEG = (CSD[2] >> 16) | (CSD[1] << 16);

  return 1;
}


int sdcard_init() {

  for (int i = 0; i < 5; i++) {
    if (sdcard_do_init()) {
      return 1;
    }
  }

  return 0;
}




// TODO this function cannot be called, causes decode_err
int pref_sdcard(int a, int b) {
  return a + (b ^ a);
}

