CFLAGS = -Wall -Wextra -std=c99

all: test_shunit

test_shunit: test_shunit.c shunit.h
	gcc $(CFLAGS) -o test_shunit test_shunit.c

clean:
	rm -f test_shunit
