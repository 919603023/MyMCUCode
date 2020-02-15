#include <stdio.h> /* FILE */
#include <fcntl.h>/* O_RDWR */
#include <string.h>/* memset() */
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <time.h>
#include <pthread.h>

typedef struct{
    long type;
    char data[128];
}MSG;
void *My_Time(void *arg)
{
    sleep(10);
    printf("失去连接");
    exit(1);
}
int main(int argc, char *argv[])
{
    pthread_t tid;
    pthread_create(&tid,NULL,My_Time,NULL);
    pthread_detach(tid);
	printf("content-type:text/html\n\n");
	char *data = getenv("QUERY_STRING");
    key_t key = ftok("./",2020);
   int msg_id = msgget(key,IPC_CREAT | 0666);
    if(msg_id < 0)
    {
        printf("error");
        return 0;
    }
        MSG msg;
        msg.type = 20;
        strcpy(msg.data,data); 
		msgsnd(msg_id,&msg,sizeof(msg)-sizeof(long),0);
        msgrcv(msg_id,&msg,sizeof(msg)-sizeof(long),10,0);
        printf("%s",msg.data);

return 0;
}


