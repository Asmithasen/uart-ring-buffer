#include "uart_driver.h"
#include "ring_buffer.h"
#include "hal_uart.h"

static ring_buffer_t rx_buffer;

void uart_driver_init(uint32_t baud_rate) {
    ring_buffer_init(&rx_buffer);
    hal_uart_init(baud_rate);
}

void uart_driver_on_rx_interrupt(uint8_t byte) {
    /* Buffer full: byte is dropped rather than blocking inside an ISR. */
    (void)ring_buffer_push(&rx_buffer, byte);
}

int uart_driver_read_byte(uint8_t *out) {
    return ring_buffer_pop(&rx_buffer, out);
}

uint16_t uart_driver_available(void) {
    return ring_buffer_count(&rx_buffer);
}

void uart_driver_write(const uint8_t *data, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
        hal_uart_send_byte(data[i]);
    }
}
