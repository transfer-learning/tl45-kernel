//
// Created by Will Gulian on 11/3/19.
//


unsigned int __mulsi3 (unsigned int a, unsigned int b)
{
  unsigned int r = 0;

  while (a)
  {
    if (a & 1)
      r += b;
    a >>= 1;
    b <<= 1;
  }
  return r;
}

typedef unsigned su_int;
typedef int si_int;

su_int __udivsi3(su_int n, su_int d) {
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

// TODO this function cannot be called, causes decode_err
int pref_soft_impl(int a, int b) {
  return a + (b ^ a);
}

