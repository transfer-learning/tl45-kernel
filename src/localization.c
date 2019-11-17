#include "localization.h"
#include "math.h"
#include "fxp.h"

static const fxp_t Q[3 * 3] = {
    (3 << 15), 0, 0,
    0, (3 << 15), 0,
    0, 0, (3 << 13)
};

static const fxp_t P0[3 * 3] = {
    (400 << 16), 0, 0,
    0, (400 << 16), 0,
    0, 0, (1 << 13)
};

void init(state_t *state) {
    memcpy(state->covariance, P0, 3 * 3);
    state->pose[0] = state->pose[1] = state->pose[2];
}

void predict(state_t *state, encoders_t *encoders) {
    fxp_t angle = state->pose[2];
    fxp_t s = fxp_sin(angle);
    fxp_t c = fxp_cos(angle);

    // encoders[i] is an integer, so we don't want fxp_mul
    fxp_t left = (encoders->left - state->last_encoders.left) * TICK;
    fxp_t right = (encoders->right - state->last_encoders.right) * TICK;
    state->last_encoders = *encoders;

    fxp_t forwards = (left + right) / 2;
    fxp_t angular = fxp_mul(right - left, ROBOT_WHEELBASE_INV);

    state->pose[0] += fxp_mul(forwards, c);
    state->pose[1] += fxp_mul(forwards, s);
    state->pose[2] += angular;

    mat_add(state->covariance, Q, state->covariance, 3, 3);
}

void update(state_t *state, measurement_t *meas) {
    // sqrt(state->pose[0]);
}
