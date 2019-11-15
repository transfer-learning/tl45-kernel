#include "scomp.h"
#include <stdint.h>

int left_encoder_read() {
    return SCOMP(0x00);
}

int right_encoder_read() {
    return SCOMP(0x00);
}

void motors_write(int left, int right) {
    SCOMP(0x00) = left;
    SCOMP(0x00) = right;
}

void sonar_enable(uint8_t sonar_bitmask) {
    SCOMP(0x00) = sonar_bitmask;
}

uint32_t sonar_read(uint8_t sonar_id) {
    return SCOMP(0xA8 + sonar_id);
}
