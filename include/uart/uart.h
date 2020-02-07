//
// Created by codetector on 2/7/20.
//

#ifndef TLOS_UART_H
#define TLOS_UART_H
#include "stdint.h"

typedef struct {
} uart_t;

uint8_t has_byte(uart_t*);
uint16_t length(uart_t*);
void write_byte(uart_t*, uint8_t byte);
uint8_t read_byte(uart_t*);


#endif //TLOS_UART_H
