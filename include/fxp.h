#pragma once
#include <stdint.h>

typedef int32_t fxp_t;

#define M_PI 3.14159265359

#define int2fxp(x) (x << 16)

fxp_t fxp_mul(fxp_t a, fxp_t b);
fxp_t fxp_div(fxp_t a, fxp_t b);
fxp_t fxp_add(fxp_t a, fxp_t b);
fxp_t fxp_sub(fxp_t a, fxp_t b);
fxp_t to_fxp(float f);
fxp_t fxp_sin(fxp_t a);
fxp_t fxp_cos(fxp_t b);
