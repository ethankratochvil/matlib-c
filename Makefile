CC      = gcc
CFLAGS  = -Wall -Wextra -Iinclude
RELEASE_FLAGS = -O3 -march=native
ASAN_FLAGS = -g -O0 -fsanitize=address -fno-omit-frame-pointer

.PHONY: all test asan clean demo

all: CFLAGS += $(RELEASE_FLAGS)
all: matrix.o test_runner demo

matrix.o: src/matrix.c
	$(CC) $(CFLAGS) -c src/matrix.c -o matrix.o

test_runner: tests/test_matrix.c src/matrix.c
	$(CC) $(CFLAGS) tests/test_matrix.c src/matrix.c -o test_runner

demo: examples/demo.c src/matrix.c
	$(CC) $(CFLAGS) examples/demo.c src/matrix.c -o demo

test: test_runner
	./test_runner

asan:
	@echo "Rebuilding with AddressSanitizer..."
	$(MAKE) clean
	$(CC) $(CFLAGS) $(ASAN_FLAGS) tests/test_matrix.c src/matrix.c -o test_runner_asan
	./test_runner_asan

clean:
	rm -f *.o test_runner demo test_runner_asan