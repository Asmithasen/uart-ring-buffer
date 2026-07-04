#include "ring_buffer.h"

#define RING_MASK (RING_BUFFER_SIZE - 1u)

void ring_buffer_init(ring_buffer_t *rb) {
    rb->head = 0;
    rb->tail = 0;
}

int ring_buffer_is_empty(const ring_buffer_t *rb) {
    return rb->head == rb->tail;
}

int ring_buffer_is_full(const ring_buffer_t *rb) {
    return (uint16_t)(rb->head - rb->tail) == RING_BUFFER_SIZE;
}

uint16_t ring_buffer_count(const ring_buffer_t *rb) {
    return (uint16_t)(rb->head - rb->tail);
}

int ring_buffer_push(ring_buffer_t *rb, uint8_t byte) {
    if (ring_buffer_is_full(rb)) {
        return -1;
    }
    rb->data[rb->head & RING_MASK] = byte;
    rb->head++;
    return 0;
}

int ring_buffer_pop(ring_buffer_t *rb, uint8_t *out) {
    if (ring_buffer_is_empty(rb)) {
        return -1;
    }
    *out = rb->data[rb->tail & RING_MASK];
    rb->tail++;
    return 0;
}
