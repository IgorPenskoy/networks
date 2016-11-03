#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <string>

static const int port = 8000;
static const char *server_addr = "127.0.0.1";
static const int max_response_buffer_size = 1024;

int main(void) {
	printf("STARTING HTTP CLIENT\n");
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
	printf("Connected to port %d\n", port);
    std::stringstream request;
    request << "GET /main.html HTTP/1.1\r\n"
    	<< "Host: " << server_addr << ":" << port << "\r\n";
    printf("Sending GET request\n\n");
	send(my_sock, request.str().c_str(), 1024, 0);
	char buf[max_response_buffer_size];
	int result = 0;
	result = recv(my_sock, buf, max_response_buffer_size, 0);
	buf[result] = '\0';
    char *buf_copy = (char *)malloc(max_response_buffer_size);
    buf_copy = strcpy(buf_copy, buf);
    char *body = strstr(buf_copy, "\r\n\r\n");
    free(buf_copy);
    printf("%s\n", body);
	printf("Success.\n");
	close(my_sock);
	return 0;
}