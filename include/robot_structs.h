#pragma once

#include <stdint.h>
#include "fxp.h"

/* Encoder values, in ticks */
typedef struct {
    int32_t left, right;
} encoders_t;

typedef struct {
    fxp_t angle;
    fxp_t dist;
} measurement_t;

typedef struct {
    fxp_t left, right;
} control_t;
