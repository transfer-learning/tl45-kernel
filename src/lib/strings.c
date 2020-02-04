//
// Created by codetector on 2/3/20.
//
#include "strings.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"

int strlen(const unsigned char *t) {
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


void *memset(void *dst, const char c, int n) {
  void *orig_dst = dst;
  if (n > 8) {

  } else {
    for (int i = 0; i < n; i++) {
      ((char *) dst)[i] = c;
    }
  }
  return orig_dst;
}


int memcmp(const int *s1, const int *s2, size_t size) {
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
#pragma clang diagnostic pop
