#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static const int port = 92574;
static const char *server_addr = "127.0.0.1";

int main(int argc, char **argv) {
	printf("TCP CLIENT\n");
	int my_sock = 0;
	my_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(my_sock < 0) {
		printf("socket() error\n");
		return -1;
	}
	struct sockaddr_in dest_addr;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(port);
	if(inet_addr(server_addr) != INADDR_NONE)
		dest_addr.sin_addr.s_addr = inet_addr(server_addr);
	else {
		printf("Invalid address %s\n", server_addr);
		close(my_sock);
		return -1;
	}
	if(connect(my_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr))) {
		printf("Connect error\n");
		close(my_sock);
		return -1;
	}
	printf("Connected to port %d, type 0 for exit\n", port);
	char value[256];
	printf("Enter name: ");
	scanf("%s", value);
	FILE* fd = fopen(value, "rb");
	if (!fd) {
		printf("Error file\n");
		exit(1);
	}
	char value2[256];
	printf("Enter copy name: ");
	scanf("%s", value2);
	send(my_sock, value2, 256, 0);
	char buf[4096];
	printf("Sending.....\n");
	size_t sz;
	while((sz = fread(buf, 1, 4096, fd)) > 0) {
		send(my_sock, buf, sz, 0);
		printf("send data %zd\n", sz);
	}
	printf("Success.\n");
	fclose(fd);
	close(my_sock);
	return 0;
}
