CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11

SRCS = ring_buffer.c uart_driver.c mock_hal_uart.c test_driver.c
OBJS = $(SRCS:.c=.o)

all: test

test: test_driver
	./test_driver

test_driver: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o test_driver

.PHONY: all test clean
