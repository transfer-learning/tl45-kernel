//
// Created by Will Gulian on 11/14/19.
//

#include "util.h"

typedef unsigned su_int;
typedef int si_int;

#define VER(i) (*((volatile int *) (0x13fc000 + (i) * 4)))

#ifdef __APPLE__
#include <stdio.h>
#define PRNT(i, x) printf("VER(%d) = %08x\n", i, (x))
#define RECV(i) read_int(i)

int read_int() {
  int a;
  fread((void *)&a, 4, 1, stdin);
  return a;
}

#else
#define PRNT(i, x) VER(i) = (x)
#define RECV(i) VER(i)
#endif

#define WHEEL_LEFT VER(5)
#define WHEEL_RIGHT VER(6)

#define ENCODER_LEFT WHEEL_LEFT
#define ENCODER_RIGHT WHEEL_RIGHT

#define SONAREN VER(7)
#define SONAR_READ(x) VER(10 + (x))

unsigned idiv(unsigned n, unsigned d);

void sim_wait(int x) {
  VER(3) = x; // set wait time
  VER(3); // wait
}

#if 1

int main() {

  SONAREN = 0x21;

  WHEEL_LEFT = -40;
  WHEEL_RIGHT = 40;

  while(1) {
//    RECV(0);

//  WHEEL_LEFT = 0;
//  WHEEL_RIGHT = 120;
//

    while (SONAR_READ(5) > 0x400);

    // start rotate

    int x = ENCODER_RIGHT;

    while (-(ENCODER_RIGHT - x) < 24000) {
      WHEEL_LEFT = -150;
      WHEEL_RIGHT = 20;
    }


    WHEEL_LEFT = -20;
    WHEEL_RIGHT = 150;

    while (SONAR_READ(5) < 0x400);

//    if (SONAR_READ(1) < 0x400 || SONAR_READ(6) < 0x400) {
//      WHEEL_LEFT = 0;
//      WHEEL_RIGHT = 0;
//    } else {
//
//      WHEEL_LEFT = -150;
//      WHEEL_RIGHT = 150;
//    }


    // sim_wait(50);

    // PRNT(0, RECV(0) / RECV(0));
  }

  int a = RECV(0);
  int b = 1363825137;
  int c = idiv(a, b);

  PRNT(0, c);

}


#else

int main() {

  while(1) {

    fxp_t a = (fxp_t) RECV(0);
    fxp_t b = (fxp_t) RECV(0);

    fxp_t c = fxp_div(a, b);

    PRNT(0,c);

    // PRNT(0, RECV(0) / RECV(0));
  }

  int a = RECV(0);
  int b = 1363825137;
  int c = idiv(a, b);

  PRNT(0, c);

}


#endif