
#include "ff14/ff.h"
#include "hardware.h"
#include "hardware_sdcard.h"
#include "lcd.h"
#include "util.h"
#include "sdcard.h"

unsigned char *nothing();

void halt();

void memcpy(volatile unsigned char *dst, volatile unsigned char *src, int size) {
  for (int i = 0; i < size; i++) {
    *(dst++) = *(src++);
  }
}

void memset(unsigned char *dst, unsigned char src, int size) {
  for (int i = 0; i < size; i++) {
    *(dst++) = src;
  }
}

unsigned basic_pref[] = {0, 0, 0, 0};

unsigned char *t = "abcdefghijk";

#define BOOL unsigned int
#define TRUE 1
#define FALSE 0

volatile int a = 0;


#define SCOMP(i) (*((volatile int *) (0x1000400 + (i) * 4)))
#define SONAREN SCOMP(0)
#define SONAR_READ(x) SCOMP(0xA8 + (x))

void collect_data() {

  char foo[20];
  SONAREN = 0xFF;

  lcd_flash("Colllecting:");

  while (SWITCHES&1 || 1) {

    int x = SONAR_READ(0);
    int r = sprint_int(foo, x);
    foo[r] = '\n';
    foo[r + 1] = '\0';

    lcd_puts(foo);
    SEG = x;

  }

}


void main() {
  int counter = 0;

  SEG = 0xdeadbeef;

  lcd_puts("LCD init\n");

  if (sdcard_init()) {
    lcd_flash("SD card init\n");
  } else {
    lcd_flash("No SD card\n");
    sad_wait(1000);
  }

  // FATFS my_fs;

//  int res = f_mount(&my_fs, "0:", 1);
//  SEG = (0xA << 28) + res;
//  if (res != FR_OK) {
//    return;
//  }

  collect_data();

//  res = f_mount(0, "", 0);
//  SEG = (0x3 << 28) + res;
//  if (res != FR_OK) {
//    return;
//  }


  halt();


}

