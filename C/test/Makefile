# BME280 Test Suite Makefile
#
# Build and run tests for the BME280 driver library

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I.. -I../mock_linux
LDFLAGS = -lm

# Source files
BME280_SRC = ../bme280.c
TEST_SRC = test_bme280.c

# Output
TEST_BIN = test_bme280

.PHONY: all clean test

all: $(TEST_BIN)

$(TEST_BIN): $(TEST_SRC) $(BME280_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -f $(TEST_BIN) *.o
