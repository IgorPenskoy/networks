#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

int main(void) {
	struct sockaddr_in sockaddr_server, sockaddr_client;
	int sock_server = 0;
	char buf[buf_size];
	if ((sock_server = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		err_write("socket()");
	memset((char *) &sockaddr_server, 0, sizeof(sockaddr_server));
	sockaddr_server.sin_family = AF_INET;
	sockaddr_server.sin_port = htons(port);
	sockaddr_server.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock_server, (struct sockaddr *)&sockaddr_server,
				sizeof(sockaddr_server)) == -1)
		err_write("bind()");
	socklen_t sockaddr_length = sizeof(struct sockaddr_in);
	if (recvfrom(sock_server, buf, buf_size, 0,
					(struct sockaddr *)&sockaddr_client, &sockaddr_length) == -1)
		err_write("recvfrom()");
	printf("Received package from %s:%d\nData: %s\n\n",
			inet_ntoa(sockaddr_client.sin_addr),
			ntohs(sockaddr_client.sin_port), buf);
	close(sock_server);
	return 0;
}
