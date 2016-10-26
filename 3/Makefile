CC := gcc
CC_FLAGS := -std=c11 -Wall -pedantic

all: client server

client:
	$(CC) $(CC_FLAGS) client.c -o client.bin

server:
	$(CC) $(CC_FLAGS) server.c -o server.bin

clean:
	rm -f *.bin
