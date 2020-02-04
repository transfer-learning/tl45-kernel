//
// Created by codetector on 2/3/20.
//
#include "hardware/system.h"
#include "hardware/peripherals.h"

void spin_wait_ms(short ms) {
  TIMERL = 0;
  while(TIMERL < (ms * 1000)){}
}
