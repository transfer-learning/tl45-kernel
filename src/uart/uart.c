//
// Created by codetector on 2/7/20.
//

#include "uart/uart.h"
#include "hardware/hardware.h"

uint8_t has_byte(uart_t* uart) {
  return (UART_CTRL & ((uint32_t) 0b111111111U)) > 0;
}

uint16_t length(uart_t* uart) {
  return (uint16_t)(UART_CTRL & ((uint32_t) 0b111111111U));
}

void write_byte(uart_t* uart, uint8_t byte) {
  UART = byte;
}

uint8_t read_byte(uart_t* uart) {
  return UART;
}
