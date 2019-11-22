//
// Created by Will Gulian on 11/17/19.
//

#include "hardware.h"
#include "lcd.h"
#include "util.h"
#include "stdint.h"
#include "sha1.h"

void quicksort(int *A, int len);

// works here
// int a[] = {4, 65, 2, -31, 0, 99, 2, 83, 782, 1};



int main() {
  lcd_reset();
  SEG = 0xdeadbeef;

  char target[] = "hello";
  char out[30];

  SHA1(out, target, 5);

  // not here
  int a[] = {4, 65, 2, -31, 0, 99, 2, 83, 782, 1};


  int n = sizeof(a) / sizeof(a[0]);
  char buf0[40];
  sprint_int(buf0, n);
  lcd_flash(buf0);

  sprint_int(buf0, a);
  lcd_flash(buf0);

  SEG = 0xb0bacafe;

  quicksort(a, n);

  SEG = 0xdeadb0ba;

  char buf[40];
  for (int i = 0; i < n; i++) {
    sprint_int(buf, a[i]);
    lcd_flash(buf);
  }

  lcd_flash(out);

  for(;;) {}
}

void quicksort(int *A, int len) {
  if (len < 2) return;

  int pivot = A[len / 2];

  int i, j;
  for (i = 0, j = len - 1; ; i++, j--) {
    while (A[i] < pivot) i++;
    while (A[j] > pivot) j--;

    SEG = 0x12345;

    if (i >= j) break;

    int temp = A[i];
    A[i]     = A[j];
    A[j]     = temp;
  }

  SEG = 134;

  quicksort(A, i);
  quicksort(A + i, len - i);

  SEG = 521;

}
