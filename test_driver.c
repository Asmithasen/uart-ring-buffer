#include <assert.h>
#include <stdio.h>
#include "ring_buffer.h"
#include "uart_driver.h"

static void test_ring_buffer_basic(void) {
    ring_buffer_t rb;
    ring_buffer_init(&rb);
    assert(ring_buffer_is_empty(&rb));
    assert(!ring_buffer_is_full(&rb));

    assert(ring_buffer_push(&rb, 0x41) == 0);
    assert(ring_buffer_count(&rb) == 1);

    uint8_t out = 0;
    assert(ring_buffer_pop(&rb, &out) == 0);
    assert(out == 0x41);
    assert(ring_buffer_is_empty(&rb));

    printf("test_ring_buffer_basic: PASS\n");
}

static void test_ring_buffer_full(void) {
    ring_buffer_t rb;
    ring_buffer_init(&rb);
    for (unsigned int i = 0; i < RING_BUFFER_SIZE; i++) {
        assert(ring_buffer_push(&rb, (uint8_t)i) == 0);
    }
    assert(ring_buffer_is_full(&rb));
    assert(ring_buffer_push(&rb, 0xFF) == -1);

    uint8_t out;
    for (unsigned int i = 0; i < RING_BUFFER_SIZE; i++) {
        assert(ring_buffer_pop(&rb, &out) == 0);
        assert(out == (uint8_t)i);
    }
    assert(ring_buffer_is_empty(&rb));

    printf("test_ring_buffer_full: PASS\n");
}

static void test_ring_buffer_wraparound(void) {
    ring_buffer_t rb;
    ring_buffer_init(&rb);
    uint8_t out;

    /* Push/pop repeatedly so head/tail advance past RING_BUFFER_SIZE
     * several times, exercising the index-masking wraparound logic. */
    for (int cycle = 0; cycle < 5; cycle++) {
        for (unsigned int i = 0; i < RING_BUFFER_SIZE / 2; i++) {
            assert(ring_buffer_push(&rb, (uint8_t)(cycle * 10 + i)) == 0);
        }
        for (unsigned int i = 0; i < RING_BUFFER_SIZE / 2; i++) {
            assert(ring_buffer_pop(&rb, &out) == 0);
            assert(out == (uint8_t)(cycle * 10 + i));
        }
    }

    printf("test_ring_buffer_wraparound: PASS\n");
}

static void test_uart_driver_rx_path(void) {
    uart_driver_init(115200);

    const char *msg = "Hi";
    for (const char *p = msg; *p; p++) {
        uart_driver_on_rx_interrupt((uint8_t)*p);
    }

    assert(uart_driver_available() == 2);

    uint8_t out;
    assert(uart_driver_read_byte(&out) == 0 && out == 'H');
    assert(uart_driver_read_byte(&out) == 0 && out == 'i');
    assert(uart_driver_read_byte(&out) == -1);

    printf("test_uart_driver_rx_path: PASS\n");
}

static void test_uart_driver_tx_path(void) {
    uart_driver_init(115200);
    printf("[expected TX output below] ");
    uart_driver_write((const uint8_t *)"OK\n", 3);
}

int main(void) {
    test_ring_buffer_basic();
    test_ring_buffer_full();
    test_ring_buffer_wraparound();
    test_uart_driver_rx_path();
    test_uart_driver_tx_path();
    printf("All tests passed.\n");
    return 0;
}
