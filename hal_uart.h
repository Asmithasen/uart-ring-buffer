#ifndef HAL_UART_H
#define HAL_UART_H

#include <stdint.h>

/* Hardware abstraction the UART driver depends on. Provide a real
 * implementation against MCU registers (e.g. STM32 USART peripheral,
 * ESP32 UART controller) to port this driver to actual hardware -
 * uart_driver.c never touches registers directly. */
void hal_uart_init(uint32_t baud_rate);
void hal_uart_send_byte(uint8_t byte);

#endif
