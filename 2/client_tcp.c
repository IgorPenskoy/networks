#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int main(void){
  int sock_client = 0;
  char buffer[512];
  struct sockaddr_in sockaddr_server;
  sock_client = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_server.sin_family = AF_INET;
  sockaddr_server.sin_port = htons(7891);
  sockaddr_server.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(sockaddr_server.sin_zero, 0, sizeof sockaddr_server.sin_zero);
  socklen_t sock_server_size = sizeof(sockaddr_server);
  connect(sock_client, (struct sockaddr *) &sockaddr_server, sock_server_size);
  recv(sock_client, buffer, 512, 0);
  printf("Data received: %s", buffer);
  return 0;
}
