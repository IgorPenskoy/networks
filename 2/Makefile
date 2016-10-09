CC := gcc
CFLAGS := -Wall -Werror -pedantic

all: client.bin server.bin

client.bin: client_tcp.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

server.bin: server_tcp.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o *.bin
