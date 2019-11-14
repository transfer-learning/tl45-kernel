#pragma once

#include "fxp.h"

typedef fxp_t mat3x3_t[3 * 3];
typedef fxp_t mat2x2_t[2 * 2];
typedef fxp_t vec3_t[3];
typedef fxp_t vec2_t[2];

#define entry(A, N, M, i, j) ((A)[(M) * (i) + (j)])

void mat_mul(fxp_t *A, fxp_t *B, fxp_t *out, int L, int N, int M);
void mat_mul_trans(fxp_t *A, fxp_t *B, fxp_t *out, int L, int N, int M);

void mat_id(fxp_t *A, int N);

void mat_add(fxp_t *A, fxp_t *B, fxp_t *out, int N, int M);
void mat_sub(fxp_t *A, fxp_t *B, fxp_t *out, int N, int M);

void mat2x2_inv(fxp_t *A, fxp_t *out);

fxp_t inner_product(fxp_t *A, fxp_t *B, int N);

fxp_t squared_distance(fxp_t *a, fxp_t *b);
fxp_t squared_norm(fxp_t *a);

void cpmat(fxp_t *in, fxp_t *out, int num);
