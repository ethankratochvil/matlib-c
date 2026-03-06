CC      = gcc
CFLAGS  = -Wall -Wextra -g -Iinclude

build: src/matrix.c
	$(CC) $(CFLAGS) -c src/matrix.c -o matrix.o

test: tests/test_matrix.c src/matrix.c
	$(CC) $(CFLAGS) tests/test_matrix.c src/matrix.c -o test_runner
	./test_runner

memcheck: tests/test_matrix.c src/matrix.c
	$(CC) $(CFLAGS) tests/test_matrix.c src/matrix.c -o test_runner
	valgrind --leak-check=full ./test_runner

clean:
	rm -f *.o test_runner demo
