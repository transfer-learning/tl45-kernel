#include "stdint.h"
#include "fxp.h"
#include "stddef.h"
#include "divmnu.h"

#include "verilator.h"

extern int32_t sluti[1024];

fxp_t fxp_mul(fxp_t a, fxp_t b) {
    uint32_t a_lo = a & ((1 << 16) - 1);
    uint32_t a_hi = a >> 16;
    uint32_t b_lo = b & ((1 << 16) - 1);
    uint32_t b_hi = b >> 16;

    return ((a_hi * b_hi) << 16) + (a_hi * b_lo + a_lo * b_hi) + ((a_lo * b_lo) >> 16);
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

fxp_t fxp_add(fxp_t a, fxp_t b) {
    return a + b;
}

fxp_t fxp_sub(fxp_t a, fxp_t b) {
    return a - b;
}

fxp_t fxp_sin(fxp_t a) {
    fxp_t rotations = fxp_div(a, to_fxp(2 * 3.14159265359));
    int16_t lookup = rotations & ((1 << 16) - 1);
    lookup >>= 6;
    return sluti[lookup & ((1 << 10) - 1)];
}

fxp_t fxp_cos(fxp_t b) {
    return fxp_sin(b + to_fxp(3.14159265359 / 2));
}



// TODO this function cannot be called, causes decode_err
int pref_fxp(int a, int b) {
  return a + (b ^ a);
}
