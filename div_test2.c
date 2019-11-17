//
// Created by Will Gulian on 11/17/19.
//

// Computes the n-m+1-halfword quotient and m-halfword remainder
// of n halfwords divided by m halfwords, unsigned.
// Max line length is 57, to fit in hacker.book.

#include <stdio.h>
#include <stddef.h>

#define BOOL int
#define TRUE 1
#define FALSE 0

#define max(x, y) ((x) > (y) ? (x) : (y))

typedef int32_t fxp_t;

__attribute__((noinline))
void print(int x) {
  printf("0x%08x\n", x);
}


int nlz(unsigned x) {
  int n;

  if (x == 0) return(32);
  n = 0;
  if (x <= 0x0000FFFF) {n = n +16; x = x <<16;}
  if (x <= 0x00FFFFFF) {n = n + 8; x = x << 8;}
  if (x <= 0x0FFFFFFF) {n = n + 4; x = x << 4;}
  if (x <= 0x3FFFFFFF) {n = n + 2; x = x << 2;}
  if (x <= 0x7FFFFFFF) {n = n + 1;}
  return n;
}

/* q[0], r[0], u[0], and v[0] contain the LEAST significant halfwords.
(The sequence is in little-endian order).

This first version is a fairly precise implementation of Knuth's
Algorithm D, for a binary computer with base b = 2**16.  The caller
supplies
   1. Space q for the quotient, m - n + 1 halfwords (at least one).
   2. Space r for the remainder (optional), n halfwords.
   3. The dividend u, m halfwords, m >= 1.
   4. The divisor v, n halfwords, n >= 2.
The most significant digit of the divisor, v[n-1], must be nonzero.  The
dividend u may have leading zeros; this just makes the algorithm take
longer and makes the quotient contain more leading zeros.  A value of
NULL may be given for the address of the remainder to signify that the
caller does not want the remainder.
   The program does not alter the input parameters u and v.
   The quotient and remainder returned may have leading zeros.  The
function itself returns a value of 0 for success and 1 for invalid
parameters (e.g., division by 0).
   For now, we must have m >= n.  Knuth's Algorithm D also requires
that the dividend be at least as long as the divisor.  (In his terms,
m >= 0 (unstated).  Therefore m+n >= n.) */

#define MAX_M 8
#define MAX_N 8

unsigned short static_vn[2 * MAX_N];
unsigned short static_un[2*(MAX_M + 1)];


__attribute__((noinline))
int divmnu(unsigned short q[], unsigned short r[],
           const unsigned short u[], const unsigned short v[],
           int m, int n) {

  const unsigned b = 65536; // Number base (16 bits).
  unsigned short *un, *vn;  // Normalized form of u, v.
  unsigned qhat;            // Estimated quotient digit.
  unsigned rhat;            // A remainder.
  unsigned p;               // Product of two digits.
  int s, i, j, t, k;

  // we statically allocate arrays for max 128 bit
  if (m > MAX_M || n > MAX_N) {
    return 1;
  }

  if (m < n || n <= 0 || v[n-1] == 0)
    return 1;              // Return if invalid param.

  if (n == 1) {                        // Take care of
    k = 0;                            // the case of a
    for (j = m - 1; j >= 0; j--) {    // single-digit
      q[j] = (k*b + u[j])/v[0];      // divisor here.
      k = (k*b + u[j]) - q[j]*v[0];
    }
    if (r != NULL) r[0] = k;
    return 0;
  }


  // Normalize by shifting v left just enough so that
  // its high-order bit is on, and shift u left the
  // same amount.  We may have to append a high-order
  // digit on the dividend; we do that unconditionally.

  s = nlz(v[n-1]) - 16;        // 0 <= s <= 15.

  vn = static_vn; // (unsigned short *)alloca(2*n);
  for (i = n - 1; i > 0; i--)
    vn[i] = (v[i] << s) | (v[i-1] >> 16-s);
  vn[0] = v[0] << s;

  un = static_un; // (unsigned short *)alloca(2*(m + 1));
  un[m] = u[m-1] >> 16-s;
  for (i = m - 1; i > 0; i--)
    un[i] = (u[i] << s) | (u[i-1] >> 16-s);
  un[0] = u[0] << s;

  for (j = m - n; j >= 0; j--) {       // Main loop.
    print(j);
    // Compute estimate qhat of q[j].
    qhat = (un[j+n]*b + un[j+n-1])/vn[n-1];
    rhat = (un[j+n]*b + un[j+n-1]) - qhat*vn[n-1];

    print(qhat);
    print(rhat);

    print(0xb0bab0ba);
    print(qhat*vn[n-2]);
    print(b*rhat + un[j+n-2]);
    print(qhat*vn[n-2] > b*rhat + un[j+n-2]);

    again:
    if (qhat >= b || qhat*vn[n-2] > b*rhat + un[j+n-2])
    { qhat = qhat - 1;
      rhat = rhat + vn[n-1];
      if (rhat < b) goto again;
    }

    print(qhat);
    print(rhat);

    // Multiply and subtract.
    k = 0;
    for (i = 0; i < n; i++) {
      p = qhat*vn[i];
      t = un[i+j] - k - (p & 0xFFFF);
      un[i+j] = t;
      k = (p >> 16) - (t >> 16);
    }
    t = un[j+n] - k;
    un[j+n] = t;

    q[j] = qhat;              // Store quotient digit.
    if (t < 0) {              // If we subtracted too
      q[j] = q[j] - 1;       // much, add back.
      k = 0;
      for (i = 0; i < n; i++) {
        t = un[i+j] + vn[i] + k;
        un[i+j] = t;
        k = t >> 16;
      }
      un[j+n] = un[j+n] + k;
    }
  } // End j.
  // If the caller wants the remainder, unnormalize
  // it and pass it back.
  if (r != NULL) {
    for (i = 0; i < n; i++)
      r[i] = (un[i] >> s) | (un[i+1] << 16-s);
  }
  return 0;
}

fxp_t fxp_div(fxp_t a_, fxp_t b_) {
  unsigned short u[4];
  unsigned short v[2];
  unsigned short q[4];

  unsigned int a = (unsigned) a_;
  unsigned int b = (unsigned) b_;

  BOOL flip_sign = (a & 0x80000000) ^ (b & 0x80000000);
  a &= ~0x80000000;
  b &= ~0x80000000;

  u[0] = 0;
  u[1] = a & 0xffff;
  u[2] = (a >> 16) & 0xffff;
  u[3] = 0;

  v[0] = b & 0xffff;
  v[1] = (b >> 16) & 0xffff;

  if (divmnu(q, NULL, u, v, 3, v[1] != 0 ? 2 : 1)) {
    // error
    return 0;
  }

  unsigned c = ((unsigned )q[0]) | ((unsigned )(q[1] << 16));
  if (flip_sign) {
    c |= 0x80000000;
  }

  return (fxp_t) c;

//    int int_part = a / b;
//    fxp_t rem = a - int_part * b;
//    if (b >> 8) {
//        return (int_part << 16) + (rem << 8) / (b >> 8);
//    }
  return 0;
}

int main() {

  print(fxp_div(0x98000, 0x50000));

}