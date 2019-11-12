#include "linalg_math.h"

void mat_mul(fxp_t *A, fxp_t *B, fxp_t *out, int L, int N, int M) {
    for (int i = 0; i < L; i++) {
        for (int k = 0; k < M; k++) {
            entry(out, L, M, i, k) = 0;
            for (int j = 0; j < N; j++) {
                entry(out, L, M, i, k) += fxp_mul(entry(A, L, N, i, j), entry(B, N, M, j, k));
            }
        }
    }
}

void mat_mul_trans(fxp_t *A, fxp_t *B, fxp_t *out, int L, int N, int M) {
    for (int i = 0; i < L; i++) {
        for (int k = 0; k < M; k++) {
            entry(out, L, M, i, k) = 0;
            for (int j = 0; j < N; j++) {
                entry(out, L, M, i, k) += fxp_mul(entry(A, L, N, i, j), entry(B, M, N, k, j));
            }
        }
    }
}

void mat_id(fxp_t *A, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            entry(A, N, N, i, j) = (i == j);
        }
    }
}

void mat_add(fxp_t *A, fxp_t *B, fxp_t *out, int N, int M) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            entry(out, N, M, i, j) = entry(A, N, M, i, j) + entry(B, N, M, i, j);
        }
    }
}

void mat_sub(fxp_t *A, fxp_t *B, fxp_t *out, int N, int M) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            entry(out, N, M, i, j) = entry(A, N, M, i, j) - entry(B, N, M, i, j);
        }
    }
}

void mat2x2_t(fxp_t *A, fxp_t *out) {
    fxp_t A00 = entry(A, 2, 2, 0, 0);
    fxp_t A01 = entry(A, 2, 2, 0, 1);
    fxp_t A10 = entry(A, 2, 2, 1, 0);
    fxp_t A11 = entry(A, 2, 2, 1, 1);

    fxp_t invdet = fxp_div(int2fxp(1), fxp_mul(A00, A11) - fxp_mul(A10, A01));
    entry(out, 2, 2, 0, 0) = fxp_mul(invdet, A11);
    entry(out, 2, 2, 1, 1) = fxp_mul(invdet, A00);
    entry(out, 2, 2, 1, 0) = -fxp_mul(invdet, A10);
    entry(out, 2, 2, 0, 1) = -fxp_mul(invdet, A01);
}
