CC      = gcc
CFLAGS  = -Wall -Wextra -g -Iinclude

.PHONY: all test memcheck clean

all: matrix.o test_runner demo

matrix.o: src/matrix.c
	$(CC) $(CFLAGS) -c src/matrix.c -o matrix.o

test_runner: tests/test_matrix.c src/matrix.c
	$(CC) $(CFLAGS) tests/test_matrix.c src/matrix.c -o test_runner

demo: examples/demo.c src/matrix.c
	$(CC) $(CFLAGS) examples/demo.c src/matrix.c -o demo

test: test_runner
	./test_runner

memcheck: test_runner
	valgrind --leak-check=full ./test_runner

clean:
	rm -f *.o test_runner demo