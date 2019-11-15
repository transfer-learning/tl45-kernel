//
// Created by Will Gulian on 11/14/19.
//

typedef unsigned su_int;
typedef int si_int;

#define VER(i) (*((volatile int *) (0x13fc000 + (i) * 4)))

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

unsigned idiv(unsigned n, unsigned d);


int main() {

  while(1) {
    PRNT(0, RECV(0));
  }

  int a = RECV(0);
  int b = 1363825137;
  int c = idiv(a, b);

  PRNT(0, c);

}

unsigned idiv(unsigned n, unsigned d) {
  const unsigned n_uword_bits = sizeof(su_int) * 8;
  su_int q;
  su_int r;
  unsigned sr;
  // special cases
  if (d == 0)
    return 0; // ?!
  if (n == 0)
    return 0;
  sr = __builtin_clz(d) - __builtin_clz(n);
  // 0 <= sr <= n_uword_bits - 1 or sr large
  if (sr > n_uword_bits - 1) // d > r
    return 0;
  if (sr == n_uword_bits - 1) // d == 1
    return n;
  ++sr;
  // 1 <= sr <= n_uword_bits - 1
  // Not a special case
  q = n << (n_uword_bits - sr);
  r = n >> sr;
  su_int carry = 0;
  for (; sr > 0; --sr) {

    // r:q = ((r:q)  << 1) | carry
    r = (r << 1) | (q >> (n_uword_bits - 1));
    q = (q << 1) | carry;

    // carry = 0;
    // if (r.all >= d.all)
    // {
    //      r.all -= d.all;
    //      carry = 1;
    // }
    const si_int s = (si_int)(d - r - 1) >> (n_uword_bits - 1);
    carry = s & 1;
    r -= d & s;
  }
  q = (q << 1) | carry;
  return q;
}
