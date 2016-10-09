CC := gcc
CFLAGS := -Wall -Werror -pedantic
HEADERS := common.h

all: client.bin server.bin

client.bin: client_udp.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

server.bin: server_udp.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o *.bin
