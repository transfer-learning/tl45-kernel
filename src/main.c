
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
#define SONAREN SCOMP(0xB2)
#define SONAR_READ(x) SCOMP(0xA8 + (x))

#define ENCODER_LEFT (*((volatile int *) (0x1000020)))
#define ENCODER_RIGHT (*((volatile int *) (0x1000024)))

void collect_data() {

  char foo[20];
  SONAREN = 0xFF;

  lcd_flash("Collecting:");

  volatile int *my_mem_dump = 1024 * 1024;

  int num_samples = 200;

  while ((SWITCHES&1) && num_samples >= 0) {
    if (SWITCHES&2) {
      SCOMP(0x83) = 256;
      SCOMP(0x8B) = -15;
    }

    SEG = num_samples;
    int r = sprint_int(foo, num_samples);
    foo[r] = '\n';
    foo[r + 1] = '\0';
    lcd_puts(foo);

    for (int i = 0; i < 8; i++) {
      int x = SONAR_READ(i);

      // SEG = x;
      *(my_mem_dump++) = x;
    }

    *(my_mem_dump++) = ENCODER_LEFT;
    *(my_mem_dump++) = ENCODER_RIGHT;

    sad_wait(100);
    num_samples--;
  }

  lcd_flash("collection done");

}

#include "fxp.h"

void main() {
  int counter = 0;

  SEG = 0xdeadbeef;



  collect_data();






//  int direction = 1;
//  int left_speed = 256;
//  int right_speed = 256;
//
//#define DEL 32
//#define MIN_SPEED 5
//
//  while (SWITCHES&1) {
//
//    if (direction == 1) {
//
//      if (left_speed <= -800) {
//        direction = -1;
//      } else if (right_speed != 256) {
//        right_speed += DEL;
//      } else {
//        left_speed -= DEL;
//      }
//
//    } else {
//
//      if (right_speed <= -800) {
//        direction = 1;
//      } else if (left_speed != 256) {
//        left_speed += DEL;
//      } else {
//        right_speed -= DEL;
//      }
//    }
//
//    SCOMP(0x83) = (left_speed < MIN_SPEED ? MIN_SPEED : left_speed);
//    SCOMP(0x8B) = -(right_speed < MIN_SPEED ? MIN_SPEED : right_speed);
//
//    SEG = (left_speed << 16) | right_speed;
//    sad_wait(300);
//
//  }

//    while (1) {
//      fxp_t foo = int_to_fxp(SWITCHES);
//
//      foo = fxp_mul(foo, int_to_fxp(8));
//
//      SEG = (foo);
//    }


//  if (SWITCHES&2) {
//    SONAREN = 0x1;
//    while(1) {
//  SEG = SONAR_READ(0);
//  sad_wait(100);
//}
//   return;
//  }


//int timer = 0;
//
//int left_speed = 256;
//int right_speed = 256;
//
//int saved_left_encoder = 0;
//int saved_right_encoder = 0;

//while (1) {
//  SCOMP(0x83) = left_speed;
//  SCOMP(0x8B) = -right_speed;
//  SEG = left_speed + right_speed;
//
//  if (timer % 100 == 0) {
//
//    if ((ENCODER_LEFT - saved_left_encoder) > (saved_right_encoder - ENCODER_RIGHT)) {
//      left_speed--;
//    } else {
//      left_speed++;
//    }
//
//    saved_left_encoder = ENCODER_LEFT;
//    saved_right_encoder = ENCODER_RIGHT;
//  }
//
//  sad_wait(1);
//  timer++;
//}
//  SONAREN = 0x1;

//  SCOMP(0x83) = 128;
//  SCOMP(0x88) = -128;

//while(1) {
//  SEG = ENCODER_LEFT;
//  sad_wait(100);
//}

  lcd_puts("LCD init\n");

//  if (sdcard_init()) {
//    lcd_flash("SD card init\n");
//  } else {
//    lcd_flash("No SD card\n");
//    sad_wait(1000);
//  }

  // FATFS my_fs;

//  int res = f_mount(&my_fs, "0:", 1);
//  SEG = (0xA << 28) + res;
//  if (res != FR_OK) {
//    return;
//  }

  // collect_data();

//  res = f_mount(0, "", 0);
//  SEG = (0x3 << 28) + res;
//  if (res != FR_OK) {
//    return;
//  }


  halt();
}

int main_unused(int a, int b) {
  return a + (a ^ b);
}
