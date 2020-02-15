#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>/* O_RDWR */
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>/* memset() */
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
typedef struct
{
    long type;
    char data[128];
} MSG;
void *My_Time(void *arg)
{
    sleep(10);
    printf("失去连接");
    exit(1);
}
int main(int argc, char *argv[])
{
    //��printf���
    printf("content-type:text/html\n\n");

    //��ȡ�������յ������� �����GET��ʽ ����ֱ��ʹ��getenv��ȡ
    //getenv�Ĳ���Ϊ��"QUERY_STRING"
    char *data = getenv("QUERY_STRING"); //data=10+20
    key_t key = ftok("My_server", 2020);

    int ret_msg = msgget(key, IPC_CREAT | 0666);
    MSG msg;
    msg.type = 20;
    strcpy(msg.data, data);
    msgsnd(ret_msg, &msg, sizeof(msg) - sizeof(long), 0);
//    msgrcv(ret_msg, &msg, sizeof(msg) - sizeof(long), 10, 0);
if(strncmp(data,"flush",5) == 0)
    printf("50:32:N:N:open");
else 
	printf("OK");
    return 0;
}
