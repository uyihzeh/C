// 服务器端
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024];

    // 创建套接字
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // 绑定IP地址和端口
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);
    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // 监听连接
    listen(server_socket, 5);

    // 接受客户端连接
    int client_addr_len = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);

    // 从客户端接收数据
    recv(client_socket, buffer, 1024, 0);
    printf("Received: %s\n", buffer);

    // 关闭套接字
    close(server_socket);
    close(client_socket);

    return 0;
}
