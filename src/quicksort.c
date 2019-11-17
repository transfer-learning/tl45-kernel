//
// Created by Will Gulian on 11/17/19.
//

#include "hardware.h"

void quicksort(int *A, int len);

// works here
int a[] = {4, 65, 2, -31, 0, 99, 2, 83, 782, 1};


int main() {
  VER0 = 0xdeadbeef;

  // not here
  int a[] = {4, 65, 2, -31, 0, 99, 2, 83, 782, 1};


  int n = sizeof a / sizeof a[0];

  VER0 = 0xb0bacafe;

  quicksort(a, n);

  VER0 = 0xdeadb0ba;

  for (int i = 0; i < n; i++) {
    VER0 = a[i];
  }



}

void quicksort(int *A, int len) {
  if (len < 2) return;

  int pivot = A[len / 2];

  int i, j;
  for (i = 0, j = len - 1; ; i++, j--) {
    while (A[i] < pivot) i++;
    while (A[j] > pivot) j--;

    VER0 = 0x12345;

    if (i >= j) break;

    int temp = A[i];
    A[i]     = A[j];
    A[j]     = temp;
  }

  VER0 = 134;

  quicksort(A, i);
  quicksort(A + i, len - i);

  VER0 = 521;

}

