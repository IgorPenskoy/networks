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
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <iterator>

static const int my_port = 8000;
static const char *server_addr = "127.0.0.1";
static const int max_client_buffer_size = 1024;

void print_info(char *info) {
	FILE *fi = fopen("info.txt", "a");
	fprintf(fi, "%s\n", info);
	fclose(fi);
}

std::string file_read(char *file_name) {
	std::string str;
    std::ifstream ifs(file_name);
    ifs >> std::noskipws;
    std::copy(std::istream_iterator<char>(ifs), std::istream_iterator<char>(), std::back_inserter(str));
    return str;
}

int main(int argc, char **argv)
{
    FILE *f = NULL;
    FILE *fi = NULL;
    if (argc != 2) {
        printf("Usage: %s %s\n", argv[0], "Path to directory with html files.");
        return -1;
    }
    fi = fopen("info.txt", "a");
    fprintf(fi, "Using direcory: %s\n", argv[1]);
    fprintf(fi, "STARTING TCP SERVER\n");
    fclose(fi);
    int listen_socket = 0;
    if((listen_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fi = fopen("info.txt", "a");
        fprintf(fi,"Error socket\n");
        fclose(fi);
        return -1;
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(my_port);
    if(inet_addr(server_addr) != INADDR_NONE)
        addr.sin_addr.s_addr = inet_addr(server_addr);
    else {
        fi = fopen("info.txt", "a");
        fprintf(fi, "Invalid address %s\n", server_addr);
        fclose(fi);
        close(listen_socket);
        return -1;
    }
    if(bind(listen_socket, (struct sockaddr *)&addr, sizeof(addr))) {
        fi = fopen("info.txt", "a");
        fprintf(fi, "Error bind\n");
        fclose(fi);
        close(listen_socket);
        return -1;
    }
    if(listen(listen_socket, SOMAXCONN)) {
        fi = fopen("info.txt", "a");
        fprintf(fi, "Error listen\n");
        fclose(fi);
        close(listen_socket);
        return -1;
    }
    fi = fopen("info.txt", "a");
    fprintf(fi, "Waiting for connection...\n");
    fclose(fi);
    int client_socket = 0;
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(struct sockaddr_in));
    socklen_t client_addr_size = sizeof(client_addr);
    int result = 0;
    char buf[max_client_buffer_size];
    for (;;result >= 0) {
        client_socket = accept(listen_socket, (struct sockaddr *) &client_addr, &client_addr_size);
        if(client_socket < 0) {
            fi = fopen("info.txt", "a");
            fprintf(fi, "Error on accept\n");
            fclose(fi);
            close(listen_socket);
            return -1;
        }
        memset(&buf, 0, max_client_buffer_size);
        result = recv(client_socket, buf, max_client_buffer_size, 0);
        std::stringstream response;
        std::stringstream response_body;
        if (result < 0) {
            fi = fopen("info.txt", "a");
            fprintf(fi,"recv failed\n");
            fclose(fi);
            close(client_socket);
        } else if (result == 0) {
            fi = fopen("info.txt", "a");
            fprintf(fi, "Connection closed\n");
            fclose(fi);
        } else if (result > 0) {
            buf[result] = '\0';
            char *buf_copy = (char *)malloc(max_client_buffer_size);
            buf_copy = strcpy(buf_copy, buf);
            char *head = strtok(buf_copy, " ");
            fi = fopen("info.txt", "a");
            fprintf(fi,"HEAD: %s\n", head);
            fclose(fi);
            if (strcmp(head, "GET") == 0) {
                char *file_name = strtok(NULL, " ");
                fi = fopen("info.txt", "a");
                fprintf(fi,"File name: %s\n", file_name);
                fclose(fi);                
                char dir[max_client_buffer_size];
                strcpy(dir, argv[1]);
                strcat(dir, file_name);
                fi = fopen("info.txt", "a");
                fprintf(fi,"Destination: %s\n", dir);
                fclose(fi);
                f = fopen(dir, "r");
                if (!f) {
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
                        << "Connetion: Closed\r\n"
                        << "Content-Length: " << response_body.str().length()
                        << "\r\n\r\n"
                        << response_body.str();
                }
                else {

                    response_body << file_read(dir);
                    response << "HTTP/1.1 200 OK\r\n"
                        << "Version: HTTP/1.1\r\n"
                        << "Content-Type: text/html; charset=utf-8\r\n"
                        << "Connetion: Closed\r\n"
                        << "Content-Length: " << response_body.str().length()
                        << "\r\n\r\n"
                        << response_body.str().c_str();
                    fclose(f);
                }
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
                    << "Connetion: Closed\r\n"
                    << "Content-Length: " << response_body.str().length()
                    << "\r\n\r\n"
                    << response_body.str();                
            }
            free(buf_copy);
            result = send(client_socket, response.str().c_str(), response.str().length(), 0);
            fi = fopen("info.txt", "a");
            fprintf(fi, "%d\n", result);
            fclose(fi);
            if (result < 0) {
                fi = fopen("info.txt", "a");
                fprintf(fi, "send failed: \n");
                fclose(f);
            }
            close(client_socket);
        }
    }
    fclose(fi);
    close(listen_socket);
    return 0;
}
