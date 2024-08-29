PROJECT=parity
CC=gcc -std=c11
FLAGS=-Wall -Werror -Wextra -Wno-missing-braces

all: server client
.PHONY: run clean

server: server.c
	$(CC) $(FLAGS) -o $@ $<

client: client.c
	$(CC) $(FLAGS) -o $@ $<

run: server
	./$<

clean:
	rm -rf server client
