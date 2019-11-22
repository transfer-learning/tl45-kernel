#include "scomp.h"
#include "hardware.h"
#include <stdint.h>

int left_encoder_read() {
    return L_ENCODER;
}

int right_encoder_read() {
    return R_ENCODER;
}

static int last_left, last_right;

void motors_write(int left, int right) {
    if (last_left != left || last_right != right) {
        SCOMP(0x83) = 0;
        SCOMP(0x8B) = 0;
    } 
    SCOMP(0x83) = left;
    SCOMP(0x8B) = -right;
}

void sonar_enable(uint8_t sonar_bitmask) {
    SCOMP(0xB2) = sonar_bitmask;
}

uint32_t sonar_read(uint8_t sonar_id) {
    return SCOMP(0xA8 + sonar_id);
}
