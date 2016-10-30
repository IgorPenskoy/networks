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

static const int my_port = 80;
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
    char buf[max_client_buffer_size];
    for (;;) {
        //Принимаем входящие соединения
        client_socket = accept(listen_socket, (struct sockaddr *) &client_addr, &client_addr_size);
        if(client_socket < 0) {
            printf("Error on accept\n");
            close(listen_socket);
            return -1;
        }
        memset(&buf, 0, max_client_buffer_size);
        int result = recv(client_socket, buf, max_client_buffer_size, 0);
        std::stringstream response; // сюда будет записываться ответ клиенту
        std::stringstream response_body; // тело ответа
        if (result < 0) {
            // ошибка получения данных
            std::cerr << "recv failed: " << result << "\n";
            close(client_socket);
        } else if (result == 0) {
            // соединение закрыто клиентом
            std::cerr << "connection closed...\n";
        } else if (result > 0) {
            // Мы знаем фактический размер полученных данных, поэтому ставим метку конца строки
            // В буфере запроса.
            buf[result] = '\0';
            // Данные успешно получены
            // формируем тело ответа (HTML)
            response_body << "<title>Test C++ HTTP Server</title>\n"
                << "<h1>Test page</h1>\n"
                << "<p>This is body of the test page...</p>\n"
                << "<h2>Request headers</h2>\n"
                << "<pre>" << buf << "</pre>\n"
                << "<em><small>Test C++ Http Server</small></em>\n";
            // Формируем весь ответ вместе с заголовками
            response << "HTTP/1.1 200 OK\r\n"
                << "Version: HTTP/1.1\r\n"
                << "Content-Type: text/html; charset=utf-8\r\n"
                << "Content-Length: " << response_body.str().length()
                << "\r\n\r\n"
                << response_body.str();
            // Отправляем ответ клиенту с помощью функции send
            result = send(client_socket, response.str().c_str(), response.str().length(), 0);
            if (result < 0) {
                // произошла ошибка при отправле данных
                std::cerr << "send failed: \n";
            }
            // Закрываем соединение к клиентом
            close(client_socket);
        }
    }
    // Убираем за собой
    close(listen_socket);
    return 0;
}
