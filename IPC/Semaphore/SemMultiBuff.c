#include "unpipc.h"

#define PATH_SEMMUTEX "/semmutex"
#define PATH_SEMEMPTY "/semempty"
#define PATH_SEMSTORE "/semstore"

#define NBUFF 20        //定义缓冲区的大小
#define MYBUFFSIZE 50

struct semMutex
{
    sem_t* semMu,*semEmpty,*semStore;//分别对缓冲区进行保护，表示缓冲区空余空间和被占据的空间从而控制生产者和消费者
   struct 
   {
       ssize_t n;
       char buff[MYBUFFSIZE];
   } data[NBUFF];
}shared;
int fd;

void *producer(void *);
void *consumer(void *);

int main(int argc,char *argv[])
{
    pthread_t pthreadProd,pthreadCons;
    if(argc != 2)
    {
        printf("please input pathname!\n");
        exit(-1);
    }
    fd = open(argv[1],O_RDONLY);

    shared.semMu = sem_open(PATH_SEMMUTEX,O_CREAT,FILE_MODE,1);
    shared.semEmpty = sem_open(PATH_SEMEMPTY,O_CREAT,FILE_MODE,NBUFF);
    shared.semStore = sem_open(PATH_SEMSTORE,O_CREAT,FILE_MODE,0);

    pthread_setconcurrency(2);      //告知系统运行的线程数目
    pthread_create(&pthreadProd,NULL,producer,NULL);        //创建线程
    pthread_create(&pthreadCons,NULL,consumer,NULL);

    pthread_join(pthreadProd,NULL); //回收线程
    pthread_join(pthreadCons,NULL);

    sem_unlink(PATH_SEMMUTEX);
    sem_unlink(PATH_SEMEMPTY);
    sem_unlink(PATH_SEMSTORE);
    exit(0);
}

void *producer(void *arg)
{
    int i = 0;
    while(1)
    {
        sem_wait(shared.semEmpty);  //这里等待的顺序很重要，否则容易造成死锁
        sem_wait(shared.semMu);
        sem_post(shared.semMu);

        if( (shared.data[i % NBUFF].n = read(fd,shared.data[i % NBUFF].buff,MYBUFFSIZE) ) == 0)
        {
            sem_post(shared.semStore);
            return NULL;
        }
        i++;
        sem_post(shared.semStore);
    }
    return NULL;
}

void * consumer(void *arg)
{
    int i = 0;
    while(1)
    {
        sem_wait(shared.semStore);
        sem_wait(shared.semMu);
        sem_post(shared.semMu);
        if(shared.data[i % NBUFF].n == 0)
          return NULL;
        write(fileno(stdout),shared.data[i % NBUFF].buff,shared.data[i % NBUFF].n);
        i++;
        sem_post(shared.semEmpty);
    }
    return NULL;
}
