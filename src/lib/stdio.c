//
// Created by codetector on 2/3/20.
//

#include "stdio.h"
#include "hardware/peripherals.h"

void puts(char *str) {
  while (*str != 0) {
    UART = *str;
    str++;
  }
}