#include "unpipc.h"

#define SEQFILE "seqfile"

//定义一个结构体，将其存储在共享内存区，包括用来同步的信号量和用来实现功能的信号
struct sharedStru{
    sem_t semMu;
    int count;
}shared;

//每个进程将进程增加1的次数
int cycletimes = 30;

int main(void)
{
    int fd,zero = 0;
    struct sharedStru *addr; //用来接收mamp返回的地址
    int i,n;

    sem_init(&shared.semMu,1,1);
    shared.count = 0;

    fd = open(SEQFILE,O_RDWR | O_CREAT,FILE_MODE); //打开文件，如果没有就新建
    write(fd,&shared,sizeof(struct sharedStru)); //将初始值写到文件中
    addr = mmap(NULL,sizeof(struct sharedStru),PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    close(fd);  //映射完毕就可以关闭描述符，不会影响映射

    setbuf(stdout,NULL);
    if( ( n = fork()) == 0)
    {
        for(i = 0;i < cycletimes;i++)
        {
            sem_wait(&addr->semMu);
            printf("pid is %d,seq num is %d\n",getpid(),addr->count++);
            sem_post(&addr->semMu);
        }
        exit(0);
    }
    for(i = 0;i < cycletimes;i++)
    {
        sem_wait(&addr->semMu);
        printf("pid is %d,seq num is %d\n",getpid(),addr->count++);
        sem_post(&addr->semMu);
    }
    exit(0);
}
