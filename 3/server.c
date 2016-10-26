#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static const int MY_PORT = 92574;
static const char *SERVERADDR = "127.0.0.1";

int main(int argc, char **argv)
{
	printf("STARTING TCP SERVER\n");
	int mysocket = 0;
	if((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Error socket\n");
		return -1;
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(MY_PORT);
	if(inet_addr(SERVERADDR) != INADDR_NONE)
		addr.sin_addr.s_addr = inet_addr(SERVERADDR);
	else {
		printf("Invalid address %s\n", SERVERADDR);
		close(mysocket);
		return -1;
	}
	if(bind(mysocket, (struct sockaddr *)&addr, sizeof(addr))) {
		printf("Error bind\n");
		close(mysocket);
		return -1;
	}
	if(listen(mysocket, 10)) {
		printf("Error listen\n");
		close(mysocket);
		return -1;
	}
	printf("Waiting for connection...\n");
	int client_socket = 0;
	struct sockaddr_in client_addr;
	socklen_t client_addr_size = sizeof(client_addr);
	client_socket = accept(mysocket, (struct sockaddr *) &client_addr, &client_addr_size);
	if(client_socket < 0) {
		printf("Error on accept\n");
		close(mysocket);
		return -1;
	}
	char value[4096];
	char name[256];
	FILE* fd;
	recv(client_socket, name, 256, 0);
	fd = fopen(name, "wb");
	if (!fd)
	{
		printf("Error file\n");
		exit(1);
	}
	int r;
	while((r = recv(client_socket, value, 4096, 0)) > 0){
		fwrite(&value, 1, r, fd);
		printf("save data %d\n", r);
	}
	printf("I have new file %s\n", name);
	close(client_socket);
	close(mysocket);
	fclose(fd);
	return 0;
}
