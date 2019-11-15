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

si_int __divsi3(si_int a, si_int b) {
  const int bits_in_word_m1 = (int)(sizeof(si_int) * 8) - 1;
  si_int s_a = a >> bits_in_word_m1; // s_a = a < 0 ? -1 : 0
  si_int s_b = b >> bits_in_word_m1; // s_b = b < 0 ? -1 : 0
  a = (a ^ s_a) - s_a;               // negate if s_a == -1
  b = (b ^ s_b) - s_b;               // negate if s_b == -1
  s_a ^= s_b;                        // sign of quotient
  //
  // On CPUs without unsigned hardware division support,
  //  this calls __udivsi3 (notice the cast to su_int).
  // On CPUs with unsigned hardware division support,
  //  this uses the unsigned division instruction.
  //
  return ((su_int)a / (su_int)b ^ s_a) - s_a; // negate if s_a == -1
}

si_int __modsi3(si_int a, si_int b) {
  return a - __divsi3(a, b) * b;
}



// TODO this function cannot be called, causes decode_err
int pref_soft_impl(int a, int b) {
  return a + (b ^ a);
}

