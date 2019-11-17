//
// Created by Will Gulian on 11/15/19.
//



int divmnu(unsigned short q[], unsigned short r[],
           const unsigned short u[], const unsigned short v[],
           int m, int n) {

  const unsigned b = 65536; // Number base (16 bits).
  unsigned short *un, *vn;  // Normalized form of u, v.


  unsigned qhat;            // Estimated quotient digit.
  unsigned rhat;            // A remainder.
  unsigned p;               // Product of two digits.
  int s, i, j, t, k;

  if (m < n || n <= 0 || v[n - 1] == 0)
    return 1;              // Return if invalid param.

  if (n == 1) {                        // Take care of
    k = 0;                            // the case of a
    for (j = m - 1; j >= 0; j--) {    // single-digit
      q[j] = (k * b + u[j]) / v[0];      // divisor here.
      k = (k * b + u[j]) - q[j] * v[0];
    }
    if (r != NULL) r[0] = k;
    return 0;
  }

  // Normalize by shifting v left just enough so that
  // its high-order bit is on, and shift u left the
  // same amount. We may have to append a high-order
  // digit on the dividend; we do that unconditionally.

  s = nlz(v[n - 1]) - 16;        // 0 <= s <= 16.
  vn = (unsigned short *) alloca(2 * n);
  for (i = n - 1; i > 0; i--)
    vn[i] = (v[i] << s) | (v[i - 1] >> 16 - s);
  vn[0] = v[0] << s;

  un = (unsigned short *) alloca(2 * (m + 1));
  un[m] = u[m - 1] >> 16 - s;
  for (i = m - 1; i > 0; i--)
    un[i] = (u[i] << s) | (u[i - 1] >> 16 - s);
  un[0] = u[0] << s;
  for (j = m - n; j[]

  rhat = rhat + vn[n - 1];
  if (rhat < b) goto again;
}

// Multiply and subtract.
k = 0;
for (
i = 0;
i<n;
i++) {
p = qhat * vn[i];
t = un[i + j] - k - (p & 0xFFFF);
un[i+j] =
t;
k = (p >> 16) - (t >> 16);
}
t = un[j + n] - k;
un[j+n] =
t;

q[j] =
qhat;              // Store quotient digit.
if (t < 0) {              // If we subtracted too
q[j] = q[j] - 1;       // much, add back.
k = 0;
for (
i = 0;
i<n;
i++) {
t = un[i + j] + vn[i] + k;
un[i+j] =
t;
k = t >> 16;
}
un[j+n] = un[j+n] +
k;
}
} // End j.
// If the caller wants the remainder, unnormalize
// it and pass it back.
if (r != NULL) {
for (
i = 0;
i<n;
i++)
r[i] = (un[i] >> s) | (un[i+1] << 16-s);
}
return 0;
}

