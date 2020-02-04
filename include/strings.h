//
// Created by Will Gulian on 10/25/19.
//

#ifndef LLVM_STRING_H
#define LLVM_STRING_H
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wincompatible-library-redeclaration"

#include "stdint.h"

int strlen(const unsigned char *t);
void *memcpy(void *dst, const void *src, int n);
void *memset(void *dst, char c, int n);
static int memcmp(const int *s1, const int *s2, size_t size);

#pragma clang diagnostic pop

#endif //LLVM_STDLIB_H
