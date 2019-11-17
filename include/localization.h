#pragma once

#include "constants.h"
#include "fxp.h"
#include "linalg_math.h"
#include "robot_structs.h"

/* The robot's current pose */
typedef struct {
    vec3_t pose;
    mat3x3_t covariance;
    encoders_t last_encoders;
} state_t;

void predict(state_t *state, encoders_t *encoders);
void update(state_t *state, measurement_t *meas);
