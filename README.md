# UART Ring Buffer Driver (C, HAL-abstracted)

A UART receive driver built around a lock-free single-producer/single-consumer
ring buffer, structured so the driver logic is testable on a PC and portable
to real hardware (STM32, ESP32, or any MCU) without changes.

## Why it's structured this way

Embedded UART drivers typically look like this on real hardware:

```
UART RX interrupt fires -> ISR reads data register -> pushes byte into a buffer
Main loop                -> polls the buffer for available bytes
```

The trick to making that testable off-target is separating **what the driver
does** (`uart_driver.c`, `ring_buffer.c` — plain, portable C) from **how it
talks to hardware** (`hal_uart.h` — an interface, implemented once per
target). This repo ships `mock_hal_uart.c`, a host-side stand-in for that
interface, so the whole driver builds and runs on Linux/macOS/Windows with no
MCU attached.

To port this to real hardware: delete `mock_hal_uart.c`, write a `hal_uart.c`
that talks to the actual UART peripheral (e.g. STM32 `USART` registers or
HAL calls, ESP32 `uart` driver), and call `uart_driver_on_rx_interrupt()`
from the real RX interrupt handler. Nothing else changes.

## Files

| File | Role |
|---|---|
| `ring_buffer.h/.c` | Fixed-size circular buffer. Single producer (ISR) writes `head`, single consumer (main loop) writes `tail` — no locks needed, safe on any architecture where aligned `uint16_t` writes are atomic (true for Cortex-M, ESP32, x86). |
| `hal_uart.h` | The hardware interface the driver depends on. |
| `mock_hal_uart.c` | Host-only implementation of `hal_uart.h`, for building/testing on a PC. |
| `uart_driver.h/.c` | Glue between the ISR-facing API and the ring buffer; also handles blocking TX. |
| `test_driver.c` | Assert-based tests: buffer basic push/pop, full/empty edge cases, index wraparound, and the driver's RX/TX paths end to end. |

## Build and test

```bash
make        # builds and runs the test suite
```

Expected output:
```
test_ring_buffer_basic: PASS
test_ring_buffer_full: PASS
test_ring_buffer_wraparound: PASS
test_uart_driver_rx_path: PASS
[expected TX output below] OK
All tests passed.
```

## Design notes

- **No dynamic allocation** — the ring buffer is a fixed-size array, sized at
  compile time (`RING_BUFFER_SIZE`, must be a power of two so index wrapping
  can use a bitmask instead of modulo).
- **ISR safety** — `uart_driver_on_rx_interrupt()` never blocks; if the
  buffer is full, the incoming byte is dropped rather than stalling the
  interrupt handler.
- **Wraparound correctness** — `head`/`tail` are unsigned 16-bit counters
  that are allowed to overflow; `head - tail` (unsigned subtraction) gives
  the correct count even across overflow, which is why the buffer size must
  stay well under 65536 and a power of two.
