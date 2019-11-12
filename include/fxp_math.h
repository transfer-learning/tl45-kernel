#pragma once
#include <stdint.h>

typedef int32_t fxp_t;

fxp_t fxp_mul(fxp_t a, fxp_t b) {
    uint32_t a_lo = a & (1 << 16 - 1);
    uint32_t a_hi = a >> 16;
    uint32_t b_lo = b & (1 << 16 - 1);
    uint32_t b_hi = b >> 16;

    return ((a_hi * b_hi) << 16) + (a_hi * b_lo + a_lo * b_hi) + ((a_lo * a_lo) >> 16);
}

fxp_t fxp_div(fxp_t a, fxp_t b) {
    int32_t integer_part = a / b;
    int32_t fractional_part = ((a - integer_part * b) << 16) / b;
    return (integer_part << 16) + (fractional_part);
}
