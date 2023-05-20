#include "unpipc.h"

#define PATH_SEMMUTEX "/semmutex"
#define PATH_SEMEMPTY "/semempty"
#define PATH_SEMSTORE "/semstore"

#define NBUFF 20        //定义缓冲区的大小

struct semMutex
{
    sem_t* semMu,*semEmpty,*semStore;//分别对缓冲区进行保护，表示缓冲区空余空间和被占据的空间从而控制生产者和消费者
    int buff[NBUFF];
}shared;
int cycleTimes = 1000;   //生产次数，也是消费次数，表示程序经过多少个生产消费过程结束

void *producer(void *);
void *consumer(void *);

int main(void)
{
    pthread_t pthreadProd,pthreadCons;

    shared.semMu = sem_open(PATH_SEMMUTEX,O_CREAT | O_EXCL,FILE_MODE,1);
    shared.semEmpty = sem_open(PATH_SEMEMPTY,O_CREAT | O_EXCL,FILE_MODE,NBUFF);
    shared.semStore = sem_open(PATH_SEMSTORE,O_CREAT | O_EXCL,FILE_MODE,0);

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
    int i;
    for(i = 0;i < cycleTimes;i++)
    {
        sem_wait(shared.semEmpty);  //这里等待的顺序很重要，否则容易造成死锁
        sem_wait(shared.semMu);
        shared.buff[i % NBUFF] = i;
        sem_post(shared.semMu);
        sem_post(shared.semStore);
    }
    return NULL;
}

void * consumer(void *arg)
{
    int i;
    for(i = 0;i < cycleTimes;i++)
    {
        sem_wait(shared.semStore);
        sem_wait(shared.semMu);
        if(shared.buff[i % NBUFF] != i)
          printf("something wrong at %d value is %d\n",i,shared.buff[i % NBUFF]);
        sem_post(shared.semMu);
        sem_post(shared.semEmpty);
    }
    return NULL;
}
