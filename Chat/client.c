#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
 
#define BUFFER_SIZE 1024
 
void *send_func();
void *recv_func();
 
/*客户端*/
int main(){
        int send_result, recv_result;
        int temp_result;
        pthread_t send_thread, recv_thread;
        //创建套接字
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        //向服务器（特定的IP和端口）发起请求
        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
        serv_addr.sin_family = AF_INET;  //使用IPv4地址
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
        serv_addr.sin_port = htons(1258);  //端口
        temp_result = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        if (temp_result == -1)
        {
                printf("连接失败\n");
                exit(1);
        } else {
                printf("连接成功\n");
                printf("----------网络聊天室----------\n");
        }
 
        //发送和接收数据，使用多线程
        send_result = pthread_create(&send_thread, NULL, send_func, (void*)(long)sock);
        recv_result = pthread_create(&recv_thread, NULL, recv_func, (void*)(long)sock);
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
 
        close(sock);
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
                if (fgets(send_buf, BUFFER_SIZE, stdin) == NULL)
                {
                        printf("no message\n");
                        continue;
                } else {
                        // 发送数据
                        if(send(sockfd, send_buf, sizeof(send_buf), 0) == -1)
                        {
                                printf("send error\n");
                                continue;
                        } else {
                                printf("-----> I : %s\n", send_buf);
                        }
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
        while (1)
        {
                // 接收数据
                if (recv(sockfd, recv_buf, sizeof(recv_buf), 0) > 0)
                {
                        printf("-----> You : %s\n", recv_buf);
                }
                // 当接收到quit时退出
                if (strncmp(recv_buf, "quit", 4) == 0)
                {
                        break;
                }
        }
}