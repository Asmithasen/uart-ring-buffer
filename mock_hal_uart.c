/* Host-side stand-in for hal_uart.h, used for testing on a PC without any
 * MCU attached. A real port (STM32/ESP32) replaces this file with one that
 * writes to actual UART registers or vendor HAL calls. */
#include "hal_uart.h"
#include <stdio.h>

void hal_uart_init(uint32_t baud_rate) {
    printf("[mock HAL] UART init at %u baud\n", baud_rate);
}

void hal_uart_send_byte(uint8_t byte) {
    putchar(byte);
}
