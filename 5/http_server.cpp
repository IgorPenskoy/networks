#include <iostream>
#include <sstream>
#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static const int my_port = 8000;
static const char *server_addr = "127.0.0.1";
static const int max_client_buffer_size = 1024;

int main(void)
{
    printf("STARTING TCP SERVER\n");
    int listen_socket = 0;
    if((listen_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error socket\n");
        return -1;
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(my_port);
    if(inet_addr(server_addr) != INADDR_NONE)
        addr.sin_addr.s_addr = inet_addr(server_addr);
    else {
        printf("Invalid address %s\n", server_addr);
        close(listen_socket);
        return -1;
    }
    if(bind(listen_socket, (struct sockaddr *)&addr, sizeof(addr))) {
        printf("Error bind\n");
        close(listen_socket);
        return -1;
    }
    if(listen(listen_socket, SOMAXCONN)) {
        printf("Error listen\n");
        close(listen_socket);
        return -1;
    }
    printf("Waiting for connection...\n");
    int client_socket = 0;
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(struct sockaddr_in));
    socklen_t client_addr_size = sizeof(client_addr);
    int result = 0;
    char buf[max_client_buffer_size];
    for (;;result >= 0) {
        client_socket = accept(listen_socket, (struct sockaddr *) &client_addr, &client_addr_size);
        if(client_socket < 0) {
            printf("Error on accept\n");
            close(listen_socket);
            return -1;
        }
        memset(&buf, 0, max_client_buffer_size);
        result = recv(client_socket, buf, max_client_buffer_size, 0);
        std::stringstream response;
        std::stringstream response_body;
        if (result < 0) {
            std::cerr << "recv failed: " << result << "\n";
            close(client_socket);
        } else if (result == 0) {
            std::cerr << "Connection closed\n";
        } else if (result > 0) {
            buf[result] = '\0';
            char *buf_copy = (char *)malloc(max_client_buffer_size);
            buf_copy = strcpy(buf_copy, buf);
            char *head = strtok(buf_copy, "\r\n");
            free(buf_copy);
            if (strcmp(head, "GET / HTTP/1.1") == 0) {
                response_body << "<title>Test C++ HTTP Server</title>\n"
                    << "<h1>Test page</h1>\n"
                    << "<p>This is body of the test page...</p>\n"
                    << "<h2>Request headers</h2>\n"
                    << "<pre>" << buf << "</pre>\n"
                    << "<em><small>Test C++ Http Server</small></em>\n";
                response << "HTTP/1.1 200 OK\r\n"
                    << "Version: HTTP/1.1\r\n"
                    << "Content-Type: text/html; charset=utf-8\r\n"
                    << "Connetion: Closed"
                    << "Content-Length: " << response_body.str().length()
                    << "\r\n\r\n"
                    << response_body.str();
            }
            else {
                response_body << "<html>\n"
                    << "<head>\n"
                    << "<title>404 Not Found</title>"
                    << "</head>\n"
                    << "<body>\n"
                    << "<h1>Not Found</h1>\n"
                    << "<p>The requested URL was not found on this server.</p>\n"
                    << "</body>\n"
                    << "</html>\n";
                response << "HTTP/1.1 400 Not Found\r\n"
                    << "Version: HTTP/1.1\r\n"
                    << "Content-Type: text/html; charset=utf-8\r\n"
                    << "Connetion: Closed"
                    << "Content-Length: " << response_body.str().length()
                    << "\r\n\r\n"
                    << response_body.str();
            }
            result = send(client_socket, response.str().c_str(), response.str().length(), 0);
            if (result < 0) {
                std::cerr << "send failed: \n";
            }
            close(client_socket);
        }
    }
    close(listen_socket);
    return 0;
}
