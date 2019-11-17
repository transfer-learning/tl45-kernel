#include <stdint.h>

void reset_encoders();

int left_encoder_read();
int right_encoder_read();

void motors_write(int left, int right);

void sonar_enable(uint8_t sonar_bitmask);
uint32_t sonar_read(uint8_t sonar_id);
