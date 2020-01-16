//
// Created by Will Gulian on 10/25/19.
//

#ifndef LLVM_STRING_H
#define LLVM_STRING_H

#include <stdint.h>

/* memcmp, memset, strlen */
static int strlen(unsigned char *t) {
  int len = 0;
  while (t[len] != '\0') {
    len++;
  }
  return len;
}

void *memcpy(void *dst, const void *src, int n) {
  void *orig_dst = dst;

  for (int i = 0; i < n; i++) {
    ((char *) dst)[i] = ((char *) src)[i];
  }

  return orig_dst;
}


void *memset(void *dst, char c, int n) {
  void *orig_dst = dst;
  if (n > 8) {
    
  } else {
    for (int i = 0; i < n; i++) {
      ((char *) dst)[i] = c;
    }
  }
  return orig_dst;
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
