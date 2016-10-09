CC := gcc
CFLAGS := -Wall -Werror -pedantic

all: client.bin server.bin

client.bin: client_udp.c
	$(CC) $(CFLAGS) $< -o $@

server.bin: server_udp.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o *.bin
