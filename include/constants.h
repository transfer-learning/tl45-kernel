#pragma once
#include "fxp.h"

#define ROBOT_WHEELBASE (to_fxp(300))
#define ROBOT_WHEELBASE_INV (to_fxp(1.0 / 300))
#define ROBOT_WHEEL     (to_fxp(101.6))

#define TICK            (to_fxp(101.6 / 9550 / 4))
#define DT              (0.01)

#define vel2motor(v)    (fxp_mul(v, to_fxp(300 / 512)))
