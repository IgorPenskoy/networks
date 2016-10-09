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
	struct sockaddr_in sockaddr_server;
	int sock_client = 0;
	char package[buf_size];
	if ((sock_client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		err_write("socket");
	memset((char *) &sockaddr_server, 0, sizeof(sockaddr_server));
	sockaddr_server.sin_family = AF_INET;
	sockaddr_server.sin_port = htons(port);
	sockaddr_server.sin_addr.s_addr = inet_addr("127.0.0.1");
	sprintf(package, "PACKAGE");
	printf("Sending package...\n");
	if (sendto(sock_client, package, buf_size, 0, (struct sockaddr *)&sockaddr_server,
					sizeof(sockaddr_server)) == -1)
		err_write("sendto()");
	close(sock_client);
	return 0;
}
