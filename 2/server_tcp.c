#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int main(void){
  int sock_server = 0;
  int sock_client = 0;
  char buffer[512];
  struct sockaddr_in sockaddr_server;
  struct sockaddr_storage sockstorage_server;
  sock_server = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_server.sin_family = AF_INET;
  sockaddr_server.sin_port = htons(7891);
  sockaddr_server.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(sockaddr_server.sin_zero, 0, sizeof sockaddr_server.sin_zero);
  bind(sock_server, (struct sockaddr *) &sockaddr_server, sizeof(sockaddr_server));
  if(listen(sock_server, 5) == 0)
    printf("Listening\n");
  else
  	 printf("Error\n");
  socklen_t sock_storage_size = sizeof(sockstorage_server);
  sock_client = accept(sock_server, (struct sockaddr *) &serverStorage, &sock_storage_size);
  strcpy(buffer,"Hello World\n");
  send(sock_client,buffer, 13, 0);
  return 0;
}
