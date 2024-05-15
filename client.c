// 客户端
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // 创建套接字
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // 设置服务器地址和端口
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    // 连接服务器
    connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // 发送数据到服务器
    strcpy(buffer, "Hello, server!");
    send(client_socket, buffer, strlen(buffer), 0);

    // 关闭套接字
    close(client_socket);
    
    return 0;
}