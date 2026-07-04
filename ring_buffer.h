#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>

#define RING_BUFFER_SIZE 64u /* must be a power of two */

typedef struct {
    uint8_t data[RING_BUFFER_SIZE];
    volatile uint16_t head; /* next write index; advanced by the producer (ISR) */
    volatile uint16_t tail; /* next read index; advanced by the consumer (main loop) */
} ring_buffer_t;

void ring_buffer_init(ring_buffer_t *rb);
int ring_buffer_push(ring_buffer_t *rb, uint8_t byte);
int ring_buffer_pop(ring_buffer_t *rb, uint8_t *out);
uint16_t ring_buffer_count(const ring_buffer_t *rb);
int ring_buffer_is_empty(const ring_buffer_t *rb);
int ring_buffer_is_full(const ring_buffer_t *rb);

#endif
