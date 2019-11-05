//
// Created by Will Gulian on 10/25/19.
//

#ifndef LLVM_STRING_H
#define LLVM_STRING_H

#include <stdint.h>

/* memcmp, memset, strlen */

static int strlen(int *t) {
  int len = 0;
  while (t[len] != '\0') {
    len++;
  }
  return len;
}

void memset(char *v, int c, unsigned int size) {
  for (int i = 0; i < size; i++) {
    *(v++) = c;
  }
}

char *memcpy(char *dst, char *src, int size) {
  char *orig = dst;
  for (int i = 0; i < size; i++) {
    *(dst++) = *(src++);
  }
  return orig;
}

static int memcmp(const int *s1, const int *s2, size_t size) {
  for (int i = 0; i < size; i++) {
    int a = *(s1++);
    int b = *(s2++);
    if (b > a) {
      return 1;
    }
    if (b < a) {
      return -1;
    }
  }
  return 0;
}


#endif //LLVM_STDLIB_H
