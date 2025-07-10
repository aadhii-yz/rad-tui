CC=gcc
CFLAGS=-Wall -Wextra -O2

.PHONY: run clean

game: game.c
	$(CC) game.c -o game $(CFLAGS)

run: game
	./game levels/level_1.txt

clean:
	rm game
