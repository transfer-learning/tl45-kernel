#pragma once
#include <stdint.h>

typedef int32_t fxp_t;

extern fxp_t sluti[1024];

#define M_PI 3.14159265359

#define int2fxp(x) ((x) << 16)
#define to_fxp(f) ((fxp_t) ((f) * (1 << 16) + 0.5))

int32_t isqrt(int32_t a);

fxp_t fxp_mul(fxp_t a, fxp_t b);
fxp_t fxp_div(fxp_t a, fxp_t b);
fxp_t fxp_add(fxp_t a, fxp_t b);
fxp_t fxp_sub(fxp_t a, fxp_t b);
fxp_t fxp_sin(fxp_t a);
fxp_t fxp_cos(fxp_t b);
fxp_t fxp_sqrt(fxp_t a);
