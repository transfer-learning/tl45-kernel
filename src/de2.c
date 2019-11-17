#include "scomp.h"
#include <stdint.h>

int left_encoder_read() {
    return *((int *)0x1000020);
}

int right_encoder_read() {
    return *((int *)0x1000024);
}

void reset_encoders() {
    *((int *)0x1000020) = 0;
    *((int *)0x1000024) = 0;
}

void motors_write(int left, int right) {
    SCOMP(0x83) = left;
    SCOMP(0x8b) = -right;
}

void sonar_enable(uint8_t sonar_bitmask) {
    SCOMP(0xb2) = sonar_bitmask;
}

uint32_t sonar_read(uint8_t sonar_id) {
    return SCOMP(0xA8 + sonar_id);
}
