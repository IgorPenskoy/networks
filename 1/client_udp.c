#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

static const char *server_ip = "127.0.0.1";

int main(void) {
	struct sockaddr_in si_other;
	int s = 0;
	char package[buf_size];
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		err_write("socket");
	memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(port);
	if (inet_pton(AF_INET, server_ip, &si_other.sin_addr) == 0)
		err_write("inet_pton()");
	sprintf(package, "PACKAGE");
	printf("Sending package...\n");
	if (sendto(s, package, buf_size, 0, (struct sockaddr *)&si_other, sizeof(si_other)) == -1)
		err_write("sendto()");
	close(s);
	return 0;
}
