#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include <stdint.h>

void uart_driver_init(uint32_t baud_rate);

/* Call this from the real UART RX interrupt handler on target hardware,
 * passing the byte just received from the data register. */
void uart_driver_on_rx_interrupt(uint8_t byte);

/* Non-blocking: returns 0 and fills *out if a byte was available, -1 if
 * the RX buffer is empty. Intended to be polled from the main loop. */
int uart_driver_read_byte(uint8_t *out);

uint16_t uart_driver_available(void);

void uart_driver_write(const uint8_t *data, uint16_t len);

#endif
