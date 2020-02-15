#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
typedef struct _msg
{
    long type;
    char data[128];
}MSG;
int ret_msg;
void My_Kill(int sig);
void *deal_client(void *arg);
void *Cgi_To_Tcp(void *arg);
int main(int argc, char const *argv[])
{
    //sockfd监听套接字（不是用来和客户端通信  只是接受客户端的链接请求）
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    key_t key = ftok("My_server",2020);
     ret_msg = msgget(key,IPC_CREAT|0666);
     if(ret_msg<0)
        perror("msgget");
    //服务器必须bind一个固定的ip port8080
    struct sockaddr_in my_addr;
    bzero(&my_addr,sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(atoi(argv[1]));
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sockfd,(struct sockaddr*)&my_addr,sizeof(my_addr));
    signal(SIGINT, My_Kill); //注册ctrl+c信号
    //使用listen 由主动变被动 创建链接队列
    listen(sockfd, 10);

    //使用accept提取已完成链接的客户端
    while(1)
    {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        //阻塞
        int new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &len);
        //new_fd 已连接套接字  代表和客户端的真正链接
        //需要查看客户端信息
        char ip_str[16]="";
        inet_ntop(AF_INET, &client_addr.sin_addr.s_addr,ip_str,16);
        printf("ip:%s  port:%hu\n", ip_str, ntohs(client_addr.sin_port));

        //创建一个单独的线程 服务于客户端
        pthread_t tid,tid1;
        pthread_create(&tid,NULL,deal_client, &new_fd);
         pthread_create(&tid1,NULL,Cgi_To_Tcp, &new_fd);
        pthread_detach(tid);
         pthread_detach(tid1);
    }

    //关闭监听套接字
    close(sockfd);
    return 0;
}

void *deal_client(void *arg)
{
    MSG msg;
    
   
    
    int new_fd = *(int *)arg;
     //和客户端通信一下（echo服务器）客户端连接服务器 并发送数据给服务器 服务器收到数据 同时转发给客户端
    while (1)
    {
    char buf[128]="";
    //收
    int ret_tcp = recv(new_fd, buf,sizeof(buf),0);
    //原样转发
    if(ret_tcp == 0)
    {
        return 0;
    }
    msg.type = 10;
    strcpy(msg.data,buf);
    printf("服务器收到：%s\n",buf);
    if(strncmp(buf,"HEAD",4) != 0){
    msgsnd(ret_msg,&msg,sizeof(msg) - sizeof(long),IPC_NOWAIT);  }
    }
    //关闭已连接套接字
    close(new_fd);
}
void *Cgi_To_Tcp(void *arg)
{
    MSG msg;

    int new_fd = *(int *)arg;
    while (1)
    {
        msgrcv(ret_msg,&msg,sizeof(msg)-sizeof(long),20,0);
        printf("服务器发出：%s\n",msg.data);
        send(new_fd,msg.data,strlen(msg.data),0);
    }
    close(new_fd);
    
}
void My_Kill(int sig) //ctrl+c退出，销毁消息队列，使网页无法操作消息队列
{
	printf("安全结束\n");
	msgctl(ret_msg, IPC_RMID, NULL);
	kill(0, 9);
}