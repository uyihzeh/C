#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
 
#define BUFFER_SIZE 1024
 
void *send_func();
void *recv_func();
 
/*服务器端*/
int main(){
        pthread_t send_thread, recv_thread;
        //创建套接字
        int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        //将套接字和IP、端口绑定
        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
        serv_addr.sin_family = AF_INET;  //使用IPv4地址
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
        serv_addr.sin_port = htons(1258);  //端口
        bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        //进入监听状态，等待用户发起请求
        listen(serv_sock, 20);
        printf("等待连接......\n");
        //接收客户端请求
        struct sockaddr_in clnt_addr;
        socklen_t clnt_addr_size = sizeof(clnt_addr);
        int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1)
        {
                printf("连接失败\n");
                exit(1);
        } else {
                printf("连接成功\n");
                printf("----------网络聊天室----------\n");
        }
        //向客户端发送和接收数据，使用线程方式
        int send_result, recv_result;
        send_result = pthread_create(&send_thread, NULL, send_func, (void*)(long)clnt_sock);
        recv_result = pthread_create(&recv_thread, NULL, recv_func, (void*)(long)clnt_sock);
        if (send_result != 0)
        {
                printf("send_thread create failure\n");
                exit(1);
        } else {
                //printf("send_thread create success\n");
        }
        if (recv_result != 0)
        {
                printf("recv_thread create failure\n");
                exit(1);
        } else {
                //printf("recv_thread create success\n");
        }
 
        send_result = pthread_join(send_thread, NULL);
        if (send_result == 0)
        {
                //printf("send_thread quit\n");
        }
        recv_result = pthread_join(recv_thread, NULL);
        if (recv_result == 0)
        {
                //printf("recv_thread quit\n");
        }
        printf("聊天结束！\n");
        //关闭套接字
        close(clnt_sock);
        close(serv_sock);
        return 0;
}
/*发送数据*/
void *send_func(void *arg)
{
        int sockfd = (int)(long)arg;
        char send_buf[BUFFER_SIZE];
        while(1)
        {
                // 从键盘输入数据
                if(fgets(send_buf, BUFFER_SIZE, stdin) == NULL)
                {
                        puts("no message\n");
                        continue;
                } else {
                        // 发送数据
                        send(sockfd, send_buf, sizeof(send_buf), 0);
                        printf("-----> I : %s\n", send_buf);
                }
                // 当输入quit时退出
                if (strncmp(send_buf, "quit", 4) == 0)
                {
                        break;
                }
        }
        pthread_exit(0);
}
/*接收数据*/
void *recv_func(void *arg)
{
        int sockfd = (int)(long)arg;
        char recv_buf[BUFFER_SIZE];
        int recv_result;
        while(1)
        {
                // 接收数据
                recv_result = recv(sockfd, recv_buf, sizeof(recv_buf), 0);
                if (recv_result > 0)
                {
                        printf("-----> You : %s\n", recv_buf);
                }
                // 当接收到quit时退出
                if (strncmp(recv_buf, "quit", 4) == 0)
                {
                        break;
                }
        }
        pthread_exit(0);
}