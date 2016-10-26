#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static const int my_port = 92574;
static const char *server_addr = "127.0.0.1";

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
	addr.sin_port = htons(my_port);
	if(inet_addr(server_addr) != INADDR_NONE)
		addr.sin_addr.s_addr = inet_addr(server_addr);
	else {
		printf("Invalid address %s\n", server_addr);
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
	FILE* fd = NULL;
	recv(client_socket, name, 256, 0);
	fd = fopen(name, "wb");
	if (!fd) {
		printf("Error file\n");
		exit(1);
	}
	int r = 0;
	while((r = recv(client_socket, value, 4096, 0)) > 0)
		fwrite(&value, 1, r, fd);
	printf("File recieved: %s\n", name);
	close(client_socket);
	close(mysocket);
	fclose(fd);
	fd = fopen(name, "r");
	if (!fd) {
		printf("Error file\n");
		exit(1);
	}
	int base[4] = {16, 8, 10, 2}, i = 0;
	char IP[40];
	while(fgets(IP, 40, fd)) {
		char *pch = strtok(IP, ".");
		while (pch) {
			int block = strtol(pch, NULL, base[i]);
			pch = strtok(NULL, ".");
			if (pch)
				printf("%d.", block);
			else
				printf("%d", block);
		}
		printf("\n");
		++i;
	}
	fclose(fd);
	return 0;
}
