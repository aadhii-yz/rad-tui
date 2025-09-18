CC=gcc
CFLAGS=-Wall -Wextra -O2

.PHONY: run clean

game: main.c game.c game.h
	$(CC) main.c -o game $(CFLAGS)

test: test.c game.c game.h
	$(CC) test.c -o test $(CFLAGS) && ./test

run: game
	./game levels/level_1.txt

clean:
	rm game
