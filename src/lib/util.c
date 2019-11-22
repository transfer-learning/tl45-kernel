//
// Created by Will Gulian on 11/3/19.
//
#include "hardware.h"

int sprint_8dig_hex(char *arr, unsigned int num) {
  for (unsigned int i = 0; i < 8; i++)
  {
    unsigned char m = (num >> ((7-i) * 4)) && 0xF;
    arr[i] = (m < 10 ? '0' : 'A' - 10) + m;
  }
  arr[8] = 0;
  return 8;
}

void wait(unsigned int ms) {
  TIMER = 0;
  while(TIMER < ms);
}

int sprint_int(char *arr, int num) {
  char foo[20];
  int i = 0;

  unsigned int my_num = (unsigned) num;

  do {
    unsigned new = my_num >> 4;//;__udivsi3(my_num, 10u);
    unsigned m = my_num - (new << 4);

    foo[i++] = (m < 10 ? '0' : 'A' - 10) + m;
    my_num = new;
  } while (my_num != 0);
  int n = i;

  while (i-- > 0) {
    *arr++ = foo[i];
  }
  *arr = 0;

  return n;
}