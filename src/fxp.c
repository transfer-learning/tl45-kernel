#include <stdint.h>
#include "fxp.h"

int32_t isqrt(int32_t a) {
    // Implementation from https://stackoverflow.com/a/5296669
    unsigned long rem = 0;
    int root = 0;
    int i;

    for (i = 0; i < 16; i++) {
        root <<= 1;
        rem <<= 2;
        rem += a >> 30;
        a <<= 2;

        if (root < rem) {
            root++;
            rem -= root;
            root++;
        }
    }

    return root >> 1;
}

fxp_t fxp_mul(fxp_t a, fxp_t b) {
    uint32_t a_lo = a & ((1 << 16) - 1);
    uint32_t a_hi = a >> 16;
    uint32_t b_lo = b & ((1 << 16) - 1);
    uint32_t b_hi = b >> 16;

    return ((a_hi * b_hi) << 16) + (a_hi * b_lo + a_lo * b_hi) + ((a_lo * b_lo) >> 16);
}

fxp_t fxp_div(fxp_t a, fxp_t b) {
    volatile int int_part = a / b;
    fxp_t rem = a - int_part * b;
    if (b >> 8) {
        return (int_part << 16) + (rem << 8) / (b >> 8);
    }
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

fxp_t fxp_sqrt(fxp_t a) {
    // sqrt(a / (2^16)) = (sqrt(a) * 2^8) / 2^16
    return isqrt(a) << 8;
}
