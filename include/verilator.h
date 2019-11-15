//
// Created by Will Gulian on 11/15/19.
//

#ifndef TL45_KERNEL_VERILATOR_H
#define TL45_KERNEL_VERILATOR_H

#include "hardware.h"

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


#endif //TL45_KERNEL_VERILATOR_H
